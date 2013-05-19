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
                    for (uint64_t i = 0; i < i.operands().size(); ++i)
                    {
                        if (!current->second.operands()[i]->match(i.operands()[i]))
                        {
                            break;
                        }
                    }

                    return *current;
                }
            }
        }

        throw "unknown mnemonic";
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

    if (opcode.mode().find(_bits32 ? mode16 : mode32) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }

    if (opcode.rm_index() == -1 && opcode.reg_index() == -1)
    {
        std::copy(opcode.code().begin(), opcode.code().end(), ret.end());

        for (auto & operand : i.operands())
        {
            auto encoded_imm = operand.encode();
            std::copy(encoded_imm.begin(), encoded_imm.end(), ret.end());
        }

        return ret;
    }

    uint8_t modrm = 0;
    uint8_t sib = 0;

    if (opcode.special_reg())
    {
        modrm |= opcode.reg_index() << 3;
    }

    else if (opcode.reg_index() != -1)
    {
        if (i.operands()[opcode.reg()]);

        modrm |= _encode_reg(i.operands()[opcode.reg()]);
    }

    if (opcode.rm_index() != -1)
    {
        auto enc = _encode_rm(_bits32 ? mode32 : mode16, i.operands()[opcode.rm_index()]);
        modrm |= enc.first;
        sib = enc.second;
    }
}

std::vector<uint8_t> reaver::assembler::lmode_generator::generate(const reaver::assembler::instruction & i)
{
    auto opcode = _find(i, bits64);

    std::vector<uint8_t> ret;

    if (opcode.mode().find(mode16) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }

    uint8_t enc_rex = 0;
}
