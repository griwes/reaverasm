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
                    for (uint64_t c = 0; c < i.operands().size(); ++c)
                    {
                        if (!current->second.operands()[c].match(i.operands()[c]))
                        {
                            break;
                        }
                    }

                    return current->second;
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

            reg["r8b"] = 0;
            reg["r8w"] = 0;
            reg["r8d"] = 0;
            reg["r8"] = 0;

            reg["r9b"] = 1;
            reg["r9w"] = 1;
            reg["r9d"] = 1;
            reg["r9"] = 1;

            reg["r10b"] = 2;
            reg["r10w"] = 2;
            reg["r10d"] = 2;
            reg["r10"] = 2;

            reg["r11b"] = 3;
            reg["r11w"] = 3;
            reg["r11d"] = 3;
            reg["r11"] = 3;

            reg["r12b"] = 4;
            reg["r12w"] = 4;
            reg["r12d"] = 4;
            reg["r12"] = 4;

            reg["r13b"] = 5;
            reg["r13w"] = 5;
            reg["r13d"] = 5;
            reg["r13"] = 5;

            reg["r14b"] = 6;
            reg["r14w"] = 6;
            reg["r14d"] = 6;
            reg["r14"] = 6;

            reg["r15b"] = 7;
            reg["r15w"] = 7;
            reg["r15d"] = 7;
            reg["r15"] = 7;

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

        return reg[op.get_register().name];
    }

    std::pair<uint8_t, uint8_t> _encode_rm(const reaver::assembler::operand & op, bool long_mode = false)
    {
        if (!op.is_register() && !op.is_address())
        {
            throw "invalid operand type for `rm` field";
        }

        if (op.is_register())
        {
            return { 0xC0 | _encode_reg(op), 0 };
        }

        const auto & address = op.get_address();

        if (address.has_base() && address.base().size() == reaver::assembler::cpu_register::byte)
        {
            throw "invalid effective address";
        }

        if (address.has_base() && address.base().size() == reaver::assembler::cpu_register::word)
        {
            if (long_mode)
            {
                throw "16 bit addressing is not available in long mode";
            }

            if ((address.has_index() && address.index().size != reaver::assembler::cpu_register::word) ||
                address.scale() != 1)
            {
                throw "invalid effective address";
            }

            switch (address.operands().size())
            {

            }
        }
    }
}

std::vector<uint8_t> reaver::assembler::pmode_generator::generate(const reaver::assembler::instruction & i)
{
    auto opcode = _find(i, _bits32 ? bits32 : bits16);

    if (opcode.operands().empty())
    {
        return opcode.code();
    }

    std::vector<uint8_t> ret;

    for (auto & x : i.operands())
    {
        if (x.has_segment_override())
        {
            const auto & name = x.get_segment_override().name;

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

    // TODO: address size override prefix, 0x67

    if (opcode.mode().find(_bits32 ? mode16 : mode32) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }

    uint8_t modrm = 0;
    uint8_t sib = 0;
    bool sib_applies = false;

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
        modrm |= enc.first & ~0x8;

        sib_applies = enc.first & 0x8;
        sib = enc.second;
    }

    std::copy(opcode.code().begin(), opcode.code().end(), ret.end());

    if (opcode.reg_index() != -1 || opcode.rm_index() != -1)
    {
        ret.push_back(modrm);
    }

    if (sib_applies)
    {
        ret.push_back(sib);
    }

    for (int8_t c = 0; static_cast<uint8_t>(c) < i.operands().size(); ++c)
    {
        if (opcode.rm_index() != c || (!opcode.special_reg() && opcode.reg_index() != c))
        {
            auto encoded_imm = i.operands()[c].encode(opcode.operands()[c].size());
            std::copy(encoded_imm.begin(), encoded_imm.end(), ret.end());
        }
    }

    return ret;
}

std::vector<uint8_t> reaver::assembler::lmode_generator::generate(const reaver::assembler::instruction & i)
{
    auto opcode = _find(i, bits64);

    std::vector<uint8_t> ret;

    for (auto & x : i.operands())
    {
        if (x.has_segment_override())
        {
            // TODO: ignored segment warnings

            auto name = x.get_segment_override().name;

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

    // TODO: address size override prefix, 0x67

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
    uint8_t sib = 0;
    bool sib_applies = false;

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
        auto enc = _encode_rm(i.operands()[opcode.rm_index()], true);
        modrm |= enc.first & ~0x8;

        sib_applies = enc.first & 0x8;
        sib = enc.second;
    }

    if (enc_rex)
    {
        ret.push_back(enc_rex);
    }

    std::copy(opcode.code().begin(), opcode.code().end(), ret.end());

    if (opcode.reg_index() != -1 || opcode.rm_index() != -1)
    {
        ret.push_back(modrm);
    }

    if (sib_applies)
    {
        ret.push_back(sib);
    }

    for (int8_t c = 0; static_cast<uint8_t>(c) < i.operands().size(); ++c)
    {
        if (opcode.rm_index() != c || (!opcode.special_reg() && opcode.reg_index() != c))
        {
            auto encoded_imm = i.operands()[c].encode(opcode.operands()[c].size());
            std::copy(encoded_imm.begin(), encoded_imm.end(), ret.end());
        }
    }

    return ret;
}
