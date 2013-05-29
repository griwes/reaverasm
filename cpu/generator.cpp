/**
 * Reaver Project Assembler License
 *
 * Copyright (C) 2013 Reaver Project Team:
 * 1. Michał "Griwes" Dominiak
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation is required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Michał "Griwes" Dominiak
 *
 **/

#include <cpu/generator.h>
#include <cpu/opcode.h>
#include <cpu/cpu.h>
#include <cpu/codepoint.h>

namespace
{
    const reaver::assembler::opcode & _find(const reaver::assembler::instruction & i, reaver::assembler::mode m)
    {
        const auto & ops = reaver::assembler::get_opcodes();

        if (ops.count(i.mnemonic()))
        {
            for (auto current = ops.lower_bound(i.mnemonic()), last = ops.upper_bound(i.mnemonic()); current != last;
                ++current)
            {
                if (current->second.operands().size() == i.operands().size())
                {
                    bool non_matched = false;

                    for (uint64_t c = 0; c < i.operands().size(); ++c)
                    {
                        if (!current->second.operands()[c].match(i.operands()[c]))
                        {
                            non_matched = true;
                            break;
                        }
                    }

                    if (!non_matched)
                    {
                        return current->second;
                    }
                }
            }

            throw "invalid combination of opcode and operands";
        }

        throw "unknown mnemonic";
    }

    uint8_t _encode_reg(const reaver::assembler::operand & op)
    {
        static std::map<std::string, uint8_t> reg;

        if (reg.empty())
        {
            reg["al"] = 0;
            reg["ah"] = 4;
            reg["ax"] = 0;
            reg["eax"] = 0;
            reg["rax"] = 0;

            reg["cl"] = 1;
            reg["ch"] = 5;
            reg["cx"] = 1;
            reg["ecx"] = 1;
            reg["rcx"] = 1;

            reg["dl"] = 2;
            reg["dh"] = 6;
            reg["dx"] = 2;
            reg["edx"] = 2;
            reg["rdx"] = 2;

            reg["bl"] = 3;
            reg["bh"] = 7;
            reg["bx"] = 3;
            reg["ebx"] = 3;
            reg["rbx"] = 3;

            reg["spl"] = 4;
            reg["sp"] = 4;
            reg["esp"] = 4;
            reg["rsp"] = 4;

            reg["bpl"] = 5;
            reg["bp"] = 5;
            reg["ebp"] = 5;
            reg["rbp"] = 5;

            reg["sil"] = 6;
            reg["si"] = 6;
            reg["esi"] = 6;
            reg["rsi"] = 6;

            reg["dil"] = 7;
            reg["di"] = 7;
            reg["edi"] = 7;
            reg["rdi"] = 7;

            reg["r8b"] = 8;
            reg["r8w"] = 8;
            reg["r8d"] = 8;
            reg["r8"] = 8;

            reg["r9b"] = 9;
            reg["r9w"] = 9;
            reg["r9d"] = 9;
            reg["r9"] = 9;

            reg["r10b"] = 10;
            reg["r10w"] = 10;
            reg["r10d"] = 10;
            reg["r10"] = 10;

            reg["r11b"] = 11;
            reg["r11w"] = 11;
            reg["r11d"] = 11;
            reg["r11"] = 11;

            reg["r12b"] = 12;
            reg["r12w"] = 12;
            reg["r12d"] = 12;
            reg["r12"] = 12;

            reg["r13b"] = 13;
            reg["r13w"] = 13;
            reg["r13d"] = 13;
            reg["r13"] = 13;

            reg["r14b"] = 14;
            reg["r14w"] = 14;
            reg["r14d"] = 14;
            reg["r14"] = 14;

            reg["r15b"] = 15;
            reg["r15w"] = 15;
            reg["r15d"] = 15;
            reg["r15"] = 15;

            reg["cs"] = 1;
            reg["ds"] = 3;
            reg["es"] = 0;
            reg["fs"] = 4;
            reg["gs"] = 5;
            reg["ss"] = 2;

            reg["cr0"] = 0;
            reg["cr2"] = 2;
            reg["cr3"] = 3;
            reg["cr4"] = 4;
            reg["cr8"] = 0;

            reg["dr0"] = 0;
            reg["dr1"] = 1;
            reg["dr2"] = 2;
            reg["dr3"] = 3;
            reg["dr4"] = 4;
            reg["dr5"] = 5;
            reg["dr6"] = 6;
            reg["dr7"] = 7;
        }

        if (!op.is_register())
        {
            throw "invalid operand type for `reg` field";
        }

        return reg[op.get_register().register_name];
    }

    std::pair<uint8_t, std::vector<reaver::assembler::codepoint>> _encode_rm(const reaver::assembler::operand & op, bool long_mode = false)
    {
        if (!op.is_register() && !op.is_address())
        {
            throw "invalid operand type for `rm` field";
        }

        if (op.is_register())
        {
            return { 3 << 6 | _encode_reg(op), {} };
        }

        const auto & address = op.get_address();

        if ((address.has_base() && address.base().size() == reaver::assembler::cpu_register::word) || address.disp().size() <= 16)
        {
            if (long_mode)
            {
                throw "16 bit addressing is not available in long mode";
            }

            static std::map<std::string, std::map<std::string, uint8_t>> rm;

            if (rm.empty())
            {
                rm["bx"]["si"] = 0;
                rm["bx"]["di"] = 1;
                rm["bp"]["si"] = 2;
                rm["bp"]["di"] = 3;
                rm["si"][""] = 4;
                rm["di"][""] = 5;
                rm["bp"][""] = 6;
                rm["bx"][""] = 7;
                rm[""][""] = 6;
            }

            if (!address.has_base())
            {
                return { rm[""][""], address.disp().encode(16) };
            }

            uint64_t disp_size = address.has_disp() ? (address.disp().size() == reaver::assembler::cpu_register::byte
                ? 8 : 16) : 0;

            return { ((disp_size / 8) << 6) | rm[address.base().register_name][address.has_index() ? address.index().register_name : ""],
                address.has_disp() ? address.disp().encode(disp_size) : std::vector<reaver::assembler::codepoint>{} };
        }

        if (address.has_base() && address.base().size() == reaver::assembler::cpu_register::qword && !long_mode)
        {
            throw "64 bit addressing is only available in long mode";
        }

        static std::map<std::string, uint8_t> rm;

        if (rm.empty())
        {
            rm["rax"] = 0;
            rm["eax"] = 0;
            rm["r8"] = 16;
            rm["r8d"] = 16;

            rm["rcx"] = 1;
            rm["ecx"] = 1;
            rm["r9"] = 17;
            rm["r9d"] = 17;

            rm["rdx"] = 2;
            rm["edx"] = 2;
            rm["r10"] = 18;
            rm["r10d"] = 18;

            rm["rbx"] = 3;
            rm["ebx"] = 3;
            rm["r11"] = 19;
            rm["r11d"] = 19;

            rm["rip"] = 5;
            rm["eip"] = 5;
            rm["rbp"] = 21;
            rm["rbp"] = 21;

            rm["r13"] = 21;
            rm["r13d"] = 21;

            rm["rsi"] = 6;
            rm["esi"] = 6;
            rm["r14"] = 22;
            rm["r14d"] = 22;

            rm["rdi"] = 7;
            rm["edi"] = 7;
            rm["r15"] = 23;
            rm["r15d"] = 23;
        }

        if (address.scale() == 0)
        {
            // no SIB

            if (!address.has_base())
            {
                if (long_mode)
                {
                    // I take that "no SIB" back

                    auto encoded = address.disp().encode(32);
                    uint8_t sib = 0x25;
                    encoded.insert(encoded.begin(), sib);

                    return { 0x4, encoded };
                }

                else
                {
                    return { 0x5, address.disp().encode(32) };
                }
            }

            else if (address.base().register_name == "rbp" || address.base().register_name == "ebp" || address.base().register_name == "r13"
                || address.base().register_name == "r13d")
            {
                if (!address.has_disp())
                {
                    return { (1 << 6) | rm[address.base().register_name], { 0 } };
                }

                return { ((address.disp().size() == 8 ? 1 : 2) << 6) | rm[address.base().register_name], address.disp()
                    .encode(address.disp().size() == 8 ? 8 : 32) };
            }

            else
            {
                if (!address.has_disp())
                {
                    return { rm[address.base().register_name], {} };
                }

                return { ((address.base().long_mode_only << 4) | (address.disp().size() == 8 ? 1 : 2) << 6)
                    | rm[address.base().register_name], address.disp().encode(address.disp().size() == 8 ? 8 : 32) };
            }
        }

        else
        {
            bool b = false, x = false;
            uint8_t sib = address.scale() << 6;
            sib |= (rm[address.index().register_name] & 7) << 3;
            sib |= rm[address.base().register_name] & 7;
            b = long_mode && (rm[address.base().register_name] & 8);
            x = long_mode && (rm[address.index().register_name] & 8);

            if (!address.has_disp())
            {
                return { 0x4 | (b << 2) | (x << 3), { sib } };
            }

            auto encoded_imm = address.disp().encode(address.disp().size() == 8 ? 8 : 32);
            encoded_imm.insert(encoded_imm.begin(), sib);

            return { ((address.disp().size() == 8 ? 1 : 2) << 6) | 0x4 | (b << 2) | (x << 3), encoded_imm };
        }
    }
}

std::vector<reaver::assembler::codepoint> reaver::assembler::pmode_generator::generate(const reaver::assembler::instruction & i)
{
    auto opcode = _find(i, _bits32 ? bits32 : bits16);

    if (opcode.operands().empty())
    {
        return opcode.code();
    }

    std::vector<codepoint> ret;

    for (auto & x : i.operands())
    {
        if (x.has_segment_override())
        {
            const auto & name = x.get_segment_override().register_name;

            if (name == "cs")
            {
                ret.push_back(0x2E);
            }

            else if (name == "ds")
            {
                ret.push_back(0x36);
            }

            else if (name == "es")
            {
                ret.push_back(0x3E);
            }

            else if (name == "fs")
            {
                ret.push_back(0x64);
            }

            else if (name == "gs")
            {
                ret.push_back(0x65);
            }

            else
            {
                throw "invalid segment override";
            }

            break;
        }
    }

    for (auto & x : i.operands())
    {
        if (x.is_address())
        {
            if (x.get_address().has_base())
            {
                if (x.get_address().base().size() == cpu_register::word)
                {
                    ret.push_back(0x67);
                }
            }

            else if (x.get_address().disp().size() == cpu_register::word)
            {
                ret.push_back(0x67);
            }
        }
    }

    if (opcode.mode().find(_bits32 ? mode16 : mode32) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }

    uint8_t modrm = 0;
    std::vector<codepoint> sibdisp;
    bool noreg = false;

    if (opcode.special_reg())
    {
        modrm |= opcode.reg_index() << 3;
    }

    else if (opcode.reg_index() != -1)
    {
        modrm |= _encode_reg(i.operands()[opcode.reg_index()]);
    }

    if (opcode.rm_index() != -1)
    {
        auto enc = _encode_rm(i.operands()[opcode.rm_index()]);
        modrm |= enc.first;

        sibdisp = enc.second;
    }

    if (opcode.mode().find(rb) != opcode.mode().end() || opcode.mode().find(rw) != opcode.mode().end() ||
        opcode.mode().find(rd) != opcode.mode().end())
    {
        ret.push_back(opcode.code()[0].code() + _encode_reg(i.operands()[0]));
        noreg = true;
    }

    else
    {
        std::copy(opcode.code().begin(), opcode.code().end(), std::back_inserter(ret));
    }

    if (opcode.reg_index() != -1 || opcode.rm_index() != -1)
    {
        ret.push_back(modrm);
    }

    if (sibdisp.size())
    {
        std::copy(sibdisp.begin(), sibdisp.end(), ret.end());
    }

    for (int8_t c = 0 + noreg; static_cast<uint8_t>(c) < i.operands().size(); ++c)
    {
        if (opcode.rm_index() != c || (!opcode.special_reg() && opcode.reg_index() != c))
        {
            if (i.operands()[c].is_register())
            {
                throw "invalid register in operands; consider this internal error.";
            }

            else
            {
                auto encoded_imm = i.operands()[c].encode(opcode.operands()[c].size());
                std::copy(encoded_imm.begin(), encoded_imm.end(), std::back_inserter(ret));
            }
        }
    }

    return ret;
}

std::vector<reaver::assembler::codepoint> reaver::assembler::lmode_generator::generate(const reaver::assembler::instruction & i)
{
    auto opcode = _find(i, bits64);

    std::vector<codepoint> ret;

    for (auto & x : i.operands())
    {
        if (x.has_segment_override())
        {
            // TODO: ignored segment warnings

            auto name = x.get_segment_override().register_name;

            if (name == "cs")
            {
                ret.push_back(0x2E);
            }

            else if (name == "ds")
            {
                ret.push_back(0x36);
            }

            else if (name == "es")
            {
                ret.push_back(0x3E);
            }

            else if (name == "fs")
            {
                ret.push_back(0x64);
            }

            else if (name == "gs")
            {
                ret.push_back(0x65);
            }

            else
            {
                throw "invalid segment override";
            }

            break;
        }
    }

    for (auto & x : i.operands())
    {
        if (x.is_address())
        {
            if (x.get_address().has_base())
            {
                if (x.get_address().base().size() == cpu_register::dword)
                {
                    ret.push_back(0x67);
                }
            }

            else if (x.get_address().disp().size() == cpu_register::word)
            {
                ret.push_back(0x67);
            }
        }
    }

    if (opcode.mode().find(mode16) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }

    uint8_t enc_rex = 0;

    if (opcode.mode().find(rex) != opcode.mode().end())
    {
        enc_rex = 0x40;
    }

    else if (opcode.mode().find(rexw) != opcode.mode().end())
    {
        enc_rex = 0x48;
    }

    uint8_t modrm = 0;
    std::vector<codepoint> sibdisp;
    bool noreg = false;

    if (opcode.special_reg())
    {
        modrm |= opcode.reg_index() << 3;
    }

    else if (opcode.reg_index() != -1)
    {
        uint8_t enc = _encode_reg(i.operands()[opcode.reg_index()]);
        modrm |= (enc & 7) << 3;

        if (enc >= 8)
        {
            enc_rex |= 4;
        }
    }

    if (opcode.rm_index() != -1)
    {
        auto enc = _encode_rm(i.operands()[opcode.rm_index()], true);
        modrm |= (enc.first & ~8);

        sibdisp = enc.second;

        if (enc.first & 56)
        {
            enc_rex |= (enc.first >> 3) & 3;
        }
    }

    if (enc_rex)
    {
        ret.push_back(enc_rex);
    }

    if (opcode.mode().find(rb) != opcode.mode().end() || opcode.mode().find(rw) != opcode.mode().end() ||
        opcode.mode().find(rd) != opcode.mode().end() || opcode.mode().find(ro) != opcode.mode().end())
    {
        ret.push_back(opcode.code()[0].code() + _encode_reg(i.operands()[0]) & 7);
        noreg = true;

        if (_encode_reg(i.operands()[0]) >= 8)
        {
            enc_rex |= 1;
        }
    }

    else
    {
        std::copy(opcode.code().begin(), opcode.code().end(), std::back_inserter(ret));
    }

    if (opcode.reg_index() != -1 || opcode.rm_index() != -1)
    {
        ret.push_back(modrm);
    }

    if (sibdisp.size())
    {
        std::copy(sibdisp.begin(), sibdisp.end(), ret.end());
    }

    for (int8_t c = 0 + noreg; static_cast<uint8_t>(c) < i.operands().size(); ++c)
    {
        if (opcode.rm_index() != c || (!opcode.special_reg() && opcode.reg_index() != c))
        {
            if (i.operands()[c].is_register())
            {
                throw "invalid register in operands; consider this internal error.";
            }

            else
            {
                auto encoded_imm = i.operands()[c].encode(opcode.operands()[c].size());
                std::copy(encoded_imm.begin(), encoded_imm.end(), std::back_inserter(ret));
            }
        }
    }

    return ret;
}
