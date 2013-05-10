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

#include <set>

#include <cpu/cpu.h>

namespace
{
    std::multimap<std::string, reaver::assembler::opcode> _opcodes;
    std::vector<std::string> _mnemonics;

    void _add(std::string name, std::vector<reaver::assembler::operand_type> operands, std::set<reaver::assembler::mode> modes,
        std::vector<uint8_t> code, uint8_t rm_index = -1, uint8_t reg_index = -1, bool special_reg = false)
    {
        _opcodes.insert(std::make_pair(name, reaver::assembler::opcode{ name, operands, modes, code, rm_index, reg_index, special_reg }));
    }
}

const std::multimap<std::string, reaver::assembler::opcode> & reaver::assembler::get_opcodes()
{
    if (_opcodes.empty())
    {
        _add("add", { al, imm8 }, { all }, { 0x04 });
        _add("add", { ax, imm16 }, { all, mode16 }, { 0x05 });
        _add("add", { eax, imm32 }, { all, mode32 }, { 0x05 });
        _add("add", { rax, imm64 }, { bits64 }, { 0x48, 0x05 });
        _add("add", { rm8, imm8 }, { all }, { 0x80 }, 0, 0, true);
        _add("add", { rm8, imm8 }, { bits64 }, { 0x40, 0x80 }, 0, 0, true);
        _add("add", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 0, true);
        _add("add", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 0, true);
        _add("add", { rm64, imm32 }, { bits64 }, { 0x48, 0x81 }, 0, 0, true);
        _add("add", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 0, true);
        _add("add", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 0, true);
        _add("add", { rm64, imm8 }, { bits64 }, { 0x40, 0x83 }, 0, 0, true);
        _add("add", { rm8, r8 }, { all }, { 0x00 }, 0, 1);
        _add("add", { rm8, r8 }, { bits64 }, { 0x00 }, 0, 1);
        _add("add", { rm16, r16 }, { all, mode16 }, { 0x01 }, 0, 1);
        _add("add", { rm32, r32 }, { all, mode32 }, { 0x01 }, 0, 1);
        _add("add", { rm64, r64 }, { bits64 }, { 0x48, 0x01 }, 0, 1);
        _add("add", { r8, rm8 }, { all }, { 0x02 }, 1, 0);
        _add("add", { r8, rm8 }, { bits64 }, { 0x02 }, 1, 0);
        _add("add", { r16, rm16 }, { all, mode16 }, { 0x03 }, 1, 0);
        _add("add", { r32, rm32 }, { all, mode32 }, { 0x03 }, 1, 0);
        _add("add", { r64, rm64 }, { bits64 }, { 0x48, 0x03 }, 1, 0);

        _add("and", { al, imm8 }, { all }, { 0x24 });
        _add("and", { ax, imm16 }, { all, mode16 }, { 0x25 });
        _add("and", { eax, imm32 }, { all, mode32 }, { 0x25 });
        _add("and", { rax, imm64 }, { bits64 }, { 0x48, 0x25 });
        _add("and", { rm8, imm8 }, { all }, { 0x80 }, 0, 4, true);
        _add("and", { rm8, imm8 }, { bits64 }, { 0x40, 0x80 }, 0, 4, true);
        _add("and", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 4, true);
        _add("and", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 4, true);
        _add("and", { rm64, imm32 }, { bits64 }, { 0x48, 0x81 }, 0, 4, true);
        _add("and", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 4, true);
        _add("and", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 4, true);
        _add("and", { rm64, imm8 }, { bits64 }, { 0x40, 0x83 }, 0, 4, true);
        _add("and", { rm8, r8 }, { all }, { 0x20 }, 0, 1);
        _add("and", { rm8, r8 }, { bits64 }, { 0x20 }, 0, 1);
        _add("and", { rm16, r16 }, { all, mode16 }, { 0x21 }, 0, 1);
        _add("and", { rm32, r32 }, { all, mode32 }, { 0x21 }, 0, 1);
        _add("and", { rm64, r64 }, { bits64 }, { 0x48, 0x21 }, 0, 1);
        _add("and", { r8, rm8 }, { all }, { 0x22 }, 1, 0);
        _add("and", { r8, rm8 }, { bits64 }, { 0x22 }, 1, 0);
        _add("and", { r16, rm16 }, { all, mode16 }, { 0x23 }, 1, 0);
        _add("and", { r32, rm32 }, { all, mode32 }, { 0x23 }, 1, 0);
        _add("and", { r64, rm64 }, { bits64 }, { 0x48, 0x23 }, 1, 0);

        _add("bt", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xA3 }, 0, 1);
        _add("bt", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xA3 }, 0, 1);
        _add("bt", { rm64, r64 }, { bits64 }, { 0x48, 0x0F, 0xA3 }, 0, 1);
        _add("bt", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 4, true);
        _add("bt", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 4, true);
        _add("bt", { rm64, imm8 }, { bits64 }, { 0x48, 0x0F, 0xBA }, 0, 4, true);

        _add("btc", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xBB }, 0, 1);
        _add("btc", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xBB }, 0, 1);
        _add("btc", { rm64, r64 }, { bits64 }, { 0x48, 0x0F, 0xBB }, 0, 1);
        _add("btc", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 7, true);
        _add("btc", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 7, true);
        _add("btc", { rm64, imm8 }, { bits64 }, { 0x48, 0x0F, 0xBA }, 0, 7, true);

        _add("btr", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xB3 }, 0, 1);
        _add("btr", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xB3 }, 0, 1);
        _add("btr", { rm64, r64 }, { bits64 }, { 0x48, 0x0F, 0xB3 }, 0, 1);
        _add("btr", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 6, true);
        _add("btr", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 6, true);
        _add("btr", { rm64, imm8 }, { bits64 }, { 0x48, 0x0F, 0xBA }, 0, 6, true);

        _add("bts", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xAB }, 0, 1);
        _add("bts", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xAB }, 0, 1);
        _add("bts", { rm64, r64 }, { bits64 }, { 0x48, 0x0F, 0xAB }, 0, 1);
        _add("bts", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 5, true);
        _add("bts", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 5, true);
        _add("bts", { rm64, imm8 }, { bits64 }, { 0x48, 0x0F, 0xBA }, 0, 5, true);

        _add("call", { rel16 }, { bits16, bits32, mode16 }, { 0xE8 }, 0);
        _add("call", { rel32 }, { all, mode32 }, { 0xE8 }, 0);
        _add("call", { rm16 }, { bits16, bits32, mode16 }, { 0xFF }, 0, 2, true);
        _add("call", { rm32 }, { bits16, bits32, mode32 }, { 0xFF }, 0, 2, true);
        _add("call", { rm64 }, { bits64 }, { 0xFF }, 0, 2, true);
        _add("call", { ptr16_16 }, { bits16, bits32, mode16 }, { 0x9A });
        _add("call", { ptr16_32 }, { bits16, bits32, mode32 }, { 0x9A });
        _add("call", { m16_16 }, { all, mode16 }, { 0xFF }, 0, 3, true);
        _add("call", { m16_32 }, { all, mode32 }, { 0xFF }, 0, 3, true);
        _add("call", { m16_64 }, { bits64 }, { 0x48, 0xFF }, 0, 3, true);

        _add("clc", {}, { all }, { 0xF8 });
        _add("cld", {}, { all }, { 0xFC });
        _add("cli", {}, { all }, { 0xFA });
        _add("clts", {}, { all }, { 0x0F, 0x06 });
        _add("cmc", {}, { all }, { 0xF5 });

        _add("cmp", { al, imm8 }, { all }, { 0x3C });
        _add("cmp", { ax, imm16 }, { all, mode16 }, { 0x3D });
        _add("cmp", { eax, imm32 }, { all, mode32 }, { 0x3D });
        _add("cmp", { rax, imm64 }, { bits64 }, { 0x48, 0x3D });
        _add("cmp", { rm8, imm8 }, { all }, { 0x80 }, 0, 7, true);
        _add("cmp", { rm8, imm8 }, { bits64 }, { 0x40, 0x80 }, 0, 7, true);
        _add("cmp", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 7, true);
        _add("cmp", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 7, true);
        _add("cmp", { rm64, imm32 }, { bits64 }, { 0x48, 0x81 }, 0, 7, true);
        _add("cmp", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 7, true);
        _add("cmp", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 7, true);
        _add("cmp", { rm64, imm8 }, { bits64 }, { 0x40, 0x83 }, 0, 7, true);
        _add("cmp", { rm8, r8 }, { all }, { 0x38 }, 0, 1);
        _add("cmp", { rm8, r8 }, { bits64 }, { 0x38 }, 0, 1);
        _add("cmp", { rm16, r16 }, { all, mode16 }, { 0x39 }, 0, 1);
        _add("cmp", { rm32, r32 }, { all, mode32 }, { 0x39 }, 0, 1);
        _add("cmp", { rm64, r64 }, { bits64 }, { 0x48, 0x39 }, 0, 1);
        _add("cmp", { r8, rm8 }, { all }, { 0x3A }, 1, 0);
        _add("cmp", { r8, rm8 }, { bits64 }, { 0x3A }, 1, 0);
        _add("cmp", { r16, rm16 }, { all, mode16 }, { 0x3B }, 1, 0);
        _add("cmp", { r32, rm32 }, { all, mode32 }, { 0x3B }, 1, 0);
        _add("cmp", { r64, rm64 }, { bits64 }, { 0x48, 0x3B }, 1, 0);

        _add("cmpxchg", { rm8, r8 }, { all }, { 0x0F, 0xB0 }, 0, 1);
        _add("cmpxchg", { rm8, r8 }, { bits64 }, { 0x40, 0x0F, 0xB0 }, 0, 1);
        _add("cmpxchg", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xB1 }, 0, 1);
        _add("cmpxchg", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xB1 }, 0, 1);
        _add("cmpxchg", { rm64, r64 }, { bits64 }, { 0x48, 0x0F, 0xB1 }, 0, 1);

        _add("cpuid", {}, { all }, { 0x0F, 0xA2 });

        _add("dec", { rm8 }, { all }, { 0xFE }, 0, 1, true);
        _add("dec", { rm8 }, { bits64 }, { 0x40, 0xFE }, 0, 1, true);
        _add("dec", { rm16 }, { all, mode16 }, { 0xFF }, 0, 1, true);
        _add("dec", { rm32 }, { all, mode32 }, { 0xFF }, 0, 1, true);
        _add("dec", { rm64 }, { bits64 }, { 0x40, 0xFF }, 0, 1, true);
        _add("dec", { r16 }, { bits16, bits32, mode16, rw }, { 0x48 });
        _add("dec", { r32 }, { bits16, bits32, mode32, rd }, { 0x48 });

        _add("div", { rm8 }, { all }, { 0xF6 }, 0, 6, true);
        _add("div", { rm8 }, { bits64 }, { 0x40, 0xF6 }, 0, 6, true);
        _add("div", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 6, true);
        _add("div", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 6, true);
        _add("div", { rm64 }, { bits64 }, { 0x40, 0xF7 }, 0, 6, true);
    }

    return _opcodes;
}

const std::vector<std::string> & reaver::assembler::get_known_mnemonics()
{
    if (_mnemonics.empty())
    {
        for (auto & op : _opcodes)
        {
            _mnemonics.push_back(op.first);
        }
    }

    return _mnemonics;
}
