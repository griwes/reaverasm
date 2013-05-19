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

    void _add(std::string name, std::vector<reaver::assembler::operand_type> && operands, std::set<reaver::assembler::mode> && modes,
        std::vector<uint8_t> && code, int8_t rm_index = -1, int8_t reg_index = -1, bool special_reg = false)
    {
        _opcodes.insert(std::make_pair(name, reaver::assembler::opcode{ name, std::move(operands), std::move(modes), std::move(code),
            rm_index, reg_index, special_reg }));
    }
}

const std::multimap<std::string, reaver::assembler::opcode> & reaver::assembler::get_opcodes()
{
    if (_opcodes.empty())
    {
        _add("add", { al, imm8 }, { all }, { 0x04 });
        _add("add", { ax, imm16 }, { all, mode16 }, { 0x05 });
        _add("add", { eax, imm32 }, { all, mode32 }, { 0x05 });
        _add("add", { rax, imm64 }, { bits64, rexw }, { 0x05 });
        _add("add", { rm8, imm8 }, { all }, { 0x80 }, 0, 0, true);
        _add("add", { rm8, imm8 }, { bits64, rex }, { 0x80 }, 0, 0, true);
        _add("add", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 0, true);
        _add("add", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 0, true);
        _add("add", { rm64, imm32 }, { bits64, rexw }, { 0x81 }, 0, 0, true);
        _add("add", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 0, true);
        _add("add", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 0, true);
        _add("add", { rm64, imm8 }, { bits64, rex }, { 0x83 }, 0, 0, true);
        _add("add", { rm8, r8 }, { all }, { 0x00 }, 0, 1);
        _add("add", { rm8, r8 }, { bits64 }, { 0x00 }, 0, 1);
        _add("add", { rm16, r16 }, { all, mode16 }, { 0x01 }, 0, 1);
        _add("add", { rm32, r32 }, { all, mode32 }, { 0x01 }, 0, 1);
        _add("add", { rm64, r64 }, { bits64, rexw }, { 0x01 }, 0, 1);
        _add("add", { r8, rm8 }, { all }, { 0x02 }, 1, 0);
        _add("add", { r8, rm8 }, { bits64 }, { 0x02 }, 1, 0);
        _add("add", { r16, rm16 }, { all, mode16 }, { 0x03 }, 1, 0);
        _add("add", { r32, rm32 }, { all, mode32 }, { 0x03 }, 1, 0);
        _add("add", { r64, rm64 }, { bits64, rexw }, { 0x03 }, 1, 0);

        _add("and", { al, imm8 }, { all }, { 0x24 });
        _add("and", { ax, imm16 }, { all, mode16 }, { 0x25 });
        _add("and", { eax, imm32 }, { all, mode32 }, { 0x25 });
        _add("and", { rax, imm64 }, { bits64, rexw }, { 0x25 });
        _add("and", { rm8, imm8 }, { all }, { 0x80 }, 0, 4, true);
        _add("and", { rm8, imm8 }, { bits64, rex }, { 0x80 }, 0, 4, true);
        _add("and", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 4, true);
        _add("and", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 4, true);
        _add("and", { rm64, imm32 }, { bits64, rexw }, { 0x81 }, 0, 4, true);
        _add("and", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 4, true);
        _add("and", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 4, true);
        _add("and", { rm64, imm8 }, { bits64, rex }, { 0x83 }, 0, 4, true);
        _add("and", { rm8, r8 }, { all }, { 0x20 }, 0, 1);
        _add("and", { rm8, r8 }, { bits64 }, { 0x20 }, 0, 1);
        _add("and", { rm16, r16 }, { all, mode16 }, { 0x21 }, 0, 1);
        _add("and", { rm32, r32 }, { all, mode32 }, { 0x21 }, 0, 1);
        _add("and", { rm64, r64 }, { bits64, rexw }, { 0x21 }, 0, 1);
        _add("and", { r8, rm8 }, { all }, { 0x22 }, 1, 0);
        _add("and", { r8, rm8 }, { bits64 }, { 0x22 }, 1, 0);
        _add("and", { r16, rm16 }, { all, mode16 }, { 0x23 }, 1, 0);
        _add("and", { r32, rm32 }, { all, mode32 }, { 0x23 }, 1, 0);
        _add("and", { r64, rm64 }, { bits64, rexw }, { 0x23 }, 1, 0);

        _add("bt", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xA3 }, 0, 1);
        _add("bt", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xA3 }, 0, 1);
        _add("bt", { rm64, r64 }, { bits64, rexw }, { 0x0F, 0xA3 }, 0, 1);
        _add("bt", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 4, true);
        _add("bt", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 4, true);
        _add("bt", { rm64, imm8 }, { bits64, rexw }, { 0x0F, 0xBA }, 0, 4, true);

        _add("btc", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xBB }, 0, 1);
        _add("btc", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xBB }, 0, 1);
        _add("btc", { rm64, r64 }, { bits64, rexw }, { 0x0F, 0xBB }, 0, 1);
        _add("btc", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 7, true);
        _add("btc", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 7, true);
        _add("btc", { rm64, imm8 }, { bits64, rexw }, { 0x0F, 0xBA }, 0, 7, true);

        _add("btr", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xB3 }, 0, 1);
        _add("btr", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xB3 }, 0, 1);
        _add("btr", { rm64, r64 }, { bits64, rexw }, { 0x0F, 0xB3 }, 0, 1);
        _add("btr", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 6, true);
        _add("btr", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 6, true);
        _add("btr", { rm64, imm8 }, { bits64, rexw }, { 0x0F, 0xBA }, 0, 6, true);

        _add("bts", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xAB }, 0, 1);
        _add("bts", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xAB }, 0, 1);
        _add("bts", { rm64, r64 }, { bits64, rexw }, { 0x0F, 0xAB }, 0, 1);
        _add("bts", { rm16, imm8 }, { all, mode16 }, { 0x0F, 0xBA }, 0, 5, true);
        _add("bts", { rm32, imm8 }, { all, mode32 }, { 0x0F, 0xBA }, 0, 5, true);
        _add("bts", { rm64, imm8 }, { bits64, rexw }, { 0x0F, 0xBA }, 0, 5, true);

        _add("call", { rel16 }, { bits16, bits32, mode16 }, { 0xE8 }, 0);
        _add("call", { rel32 }, { all, mode32 }, { 0xE8 }, 0);
        _add("call", { rm16 }, { bits16, bits32, mode16 }, { 0xFF }, 0, 2, true);
        _add("call", { rm32 }, { bits16, bits32, mode32 }, { 0xFF }, 0, 2, true);
        _add("call", { rm64 }, { bits64 }, { 0xFF }, 0, 2, true);
        _add("call", { ptr16_16 }, { bits16, bits32, mode16 }, { 0x9A });
        _add("call", { ptr16_32 }, { bits16, bits32, mode32 }, { 0x9A });
        _add("call", { m16_16 }, { all, mode16 }, { 0xFF }, 0, 3, true);
        _add("call", { m16_32 }, { all, mode32 }, { 0xFF }, 0, 3, true);
        _add("call", { m16_64 }, { bits64, rexw }, { 0xFF }, 0, 3, true);

        _add("clc", {}, { all }, { 0xF8 });
        _add("cld", {}, { all }, { 0xFC });
        _add("cli", {}, { all }, { 0xFA });
        _add("clts", {}, { all }, { 0x0F, 0x06 });
        _add("cmc", {}, { all }, { 0xF5 });

        _add("cmp", { al, imm8 }, { all }, { 0x3C });
        _add("cmp", { ax, imm16 }, { all, mode16 }, { 0x3D });
        _add("cmp", { eax, imm32 }, { all, mode32 }, { 0x3D });
        _add("cmp", { rax, imm64 }, { bits64, rexw }, { 0x3D });
        _add("cmp", { rm8, imm8 }, { all }, { 0x80 }, 0, 7, true);
        _add("cmp", { rm8, imm8 }, { bits64, rex }, { 0x80 }, 0, 7, true);
        _add("cmp", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 7, true);
        _add("cmp", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 7, true);
        _add("cmp", { rm64, imm32 }, { bits64, rexw }, { 0x81 }, 0, 7, true);
        _add("cmp", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 7, true);
        _add("cmp", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 7, true);
        _add("cmp", { rm64, imm8 }, { bits64, rex }, { 0x83 }, 0, 7, true);
        _add("cmp", { rm8, r8 }, { all }, { 0x38 }, 0, 1);
        _add("cmp", { rm8, r8 }, { bits64 }, { 0x38 }, 0, 1);
        _add("cmp", { rm16, r16 }, { all, mode16 }, { 0x39 }, 0, 1);
        _add("cmp", { rm32, r32 }, { all, mode32 }, { 0x39 }, 0, 1);
        _add("cmp", { rm64, r64 }, { bits64, rexw }, { 0x39 }, 0, 1);
        _add("cmp", { r8, rm8 }, { all }, { 0x3A }, 1, 0);
        _add("cmp", { r8, rm8 }, { bits64 }, { 0x3A }, 1, 0);
        _add("cmp", { r16, rm16 }, { all, mode16 }, { 0x3B }, 1, 0);
        _add("cmp", { r32, rm32 }, { all, mode32 }, { 0x3B }, 1, 0);
        _add("cmp", { r64, rm64 }, { bits64, rexw }, { 0x3B }, 1, 0);

        _add("cmpxchg", { rm8, r8 }, { all }, { 0x0F, 0xB0 }, 0, 1);
        _add("cmpxchg", { rm8, r8 }, { bits64, rex }, { 0x0F, 0xB0 }, 0, 1);
        _add("cmpxchg", { rm16, r16 }, { all, mode16 }, { 0x0F, 0xB1 }, 0, 1);
        _add("cmpxchg", { rm32, r32 }, { all, mode32 }, { 0x0F, 0xB1 }, 0, 1);
        _add("cmpxchg", { rm64, r64 }, { bits64, rexw }, { 0x0F, 0xB1 }, 0, 1);

        _add("cpuid", {}, { all }, { 0x0F, 0xA2 });

        _add("dec", { rm8 }, { all }, { 0xFE }, 0, 1, true);
        _add("dec", { rm8 }, { bits64, rex }, { 0xFE }, 0, 1, true);
        _add("dec", { rm16 }, { all, mode16 }, { 0xFF }, 0, 1, true);
        _add("dec", { rm32 }, { all, mode32 }, { 0xFF }, 0, 1, true);
        _add("dec", { rm64 }, { bits64, rex }, { 0xFF }, 0, 1, true);
        _add("dec", { r16 }, { bits16, bits32, mode16, rw }, { 0x48 });
        _add("dec", { r32 }, { bits16, bits32, mode32, rd }, { 0x48 });

        _add("div", { rm8 }, { all }, { 0xF6 }, 0, 6, true);
        _add("div", { rm8 }, { bits64, rex }, { 0xF6 }, 0, 6, true);
        _add("div", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 6, true);
        _add("div", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 6, true);
        _add("div", { rm64 }, { bits64, rex }, { 0xF7 }, 0, 6, true);

        _add("enter", { imm16, imm8 }, { all }, { 0xC8 });

        _add("hlt", {}, { all }, { 0xF4 });

        _add("idiv", { rm8 }, { all }, { 0xF6 }, 0, 7, true);
        _add("idiv", { rm8 }, { bits64, rex }, { 0xF6 }, 0, 7, true);
        _add("idiv", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 7, true);
        _add("idiv", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 7, true);
        _add("idiv", { rm64 }, { bits64, rex }, { 0xF7 }, 0, 7, true);

        _add("imul", { rm8 }, { all }, { 0xF6 }, 0, 5, true);
        _add("imul", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 5, true);
        _add("imul", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 5, true);
        _add("imul", { rm64 }, { bits64, rexw }, { 0xF7 }, 0, 5, true);
        _add("imul", { r16, rm16 }, { all, mode16 }, { 0x0F, 0xAF }, 1, 0);
        _add("imul", { r32, rm32 }, { all, mode32 }, { 0x0F, 0xAF }, 1, 0);
        _add("imul", { r64, rm64 }, { bits64, rexw }, { 0x0F, 0xAF }, 1, 0);
        _add("imul", { r16, rm16, imm8 }, { all, mode16 }, { 0x6B }, 1, 0);
        _add("imul", { r32, rm32, imm8 }, { all, mode32 }, { 0x6B }, 1, 0);
        _add("imul", { r64, rm64, imm8 }, { bits64, rexw }, { 0x6B }, 1, 0);
        _add("imul", { r16, rm16, imm16 }, { all, mode16 }, { 0x69 }, 1, 0);
        _add("imul", { r32, rm32, imm32 }, { all, mode32 }, { 0x69 }, 1, 0);
        _add("imul", { r64, rm64, imm32 }, { bits64, rexw }, { 0x69 }, 1, 0);

        _add("in", { al, imm8 }, { all }, { 0xE4 });
        _add("in", { ax, imm8 }, { all, mode16 }, { 0xE5 });
        _add("in", { eax, imm8 }, { all, mode32 }, { 0xE5 });
        _add("in", { al, dx }, { all }, { 0xEC });
        _add("in", { ax, dx }, { all, mode16 }, { 0xED });
        _add("in", { eax, dx }, { all, mode32 }, { 0xED });

        _add("inc", { rm8 }, { all }, { 0xFE }, 0, 0, true);
        _add("inc", { rm8 }, { bits64, rex }, { 0xFE }, 0, 0, true);
        _add("inc", { rm16 }, { all, mode16 }, { 0xFF }, 0, 0, true);
        _add("inc", { rm32 }, { all, mode32 }, { 0xFF }, 0, 0, true);
        _add("inc", { rm64 }, { bits64, rex }, { 0xFF }, 0, 0, true);
        _add("inc", { r16 }, { bits16, bits32, mode16, rw }, { 0x40 });
        _add("inc", { r32 }, { bits16, bits32, mode32, rd }, { 0x40 });

        _add("int3", {}, { all }, { 0xCC });
        _add("int", { imm8 }, { all }, { 0xCD });
        _add("into", {}, { all }, { 0xCE });

        _add("iret", {}, { all }, { 0xCF });
        _add("iretd", {}, { all }, { 0xCF });
        _add("iretq", {}, { all, rexw }, { 0xCF });

        _add("ja", { rel8 }, { all }, { 0x77 });
        _add("jae", { rel8 }, { all }, { 0x73 });
        _add("jb", { rel8 }, { all }, { 0x72 });
        _add("jbe", { rel8 }, { all }, { 0x76 });
        _add("jc", { rel8 }, { all }, { 0x72 });
        _add("jcxz", { rel8 }, { bits16, bits32, mode16 }, { 0xE3 });
        _add("jecxz", { rel8 }, { bits16, bits32, mode32 }, { 0xE3 });
        _add("jecxz", { rel8 }, { bits64, mode32 }, { 0xE3 });
        _add("jrcxz", { rel8 }, { bits64 }, { 0xE3 });
        _add("je", { rel8 }, { all }, { 0x74 });
        _add("jg", { rel8 }, { all }, { 0x7F });
        _add("jge", { rel8 }, { all }, { 0x7D });
        _add("jl", { rel8 }, { all }, { 0x7C });
        _add("jle", { rel8 }, { all }, { 0x7E });
        _add("jna", { rel8 }, { all }, { 0x76 });
        _add("jnae", { rel8 }, { all }, { 0x72 });
        _add("jnb", { rel8 }, { all }, { 0x73 });
        _add("jnbe", { rel8 }, { all }, { 0x77 });
        _add("jnc", { rel8 }, { all }, { 0x73 });
        _add("jne", { rel8 }, { all }, { 0x75 });
        _add("jng", { rel8 }, { all }, { 0x7E });
        _add("jnge", { rel8 }, { all }, { 0x7C });
        _add("jnl", { rel8 }, { all }, { 0x7D });
        _add("jnle", { rel8 }, { all }, { 0x7F });
        _add("jno", { rel8 }, { all }, { 0x71 });
        _add("jnp", { rel8 }, { all }, { 0x7B });
        _add("jns", { rel8 }, { all }, { 0x79 });
        _add("jnz", { rel8 }, { all }, { 0x75 });
        _add("jo", { rel8 }, { all }, { 0x70 });
        _add("jp", { rel8 }, { all }, { 0x7A });
        _add("jpe", { rel8 }, { all }, { 0x7A });
        _add("jpo", { rel8 }, { all }, { 0x7B });
        _add("js", { rel8 }, { all }, { 0x78 });
        _add("jz", { rel8 }, { all }, { 0x74 });

        _add("ja", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x87 });
        _add("jae", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x83 });
        _add("jb", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x82 });
        _add("jbe", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x86 });
        _add("jc", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x82 });
        _add("je", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x84 });
        _add("jg", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8F });
        _add("jge", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8D });
        _add("jl", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8C });
        _add("jle", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8E });
        _add("jna", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x86 });
        _add("jnae", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x82 });
        _add("jnb", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x83 });
        _add("jnbe", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x87 });
        _add("jnc", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x83 });
        _add("jne", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x85 });
        _add("jng", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8E });
        _add("jnge", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8C });
        _add("jnl", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8D });
        _add("jnle", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8F });
        _add("jno", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x81 });
        _add("jnp", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8B });
        _add("jns", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x89 });
        _add("jnz", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x85 });
        _add("jo", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x80 });
        _add("jp", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8A });
        _add("jpe", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8A });
        _add("jpo", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x8B });
        _add("js", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x88 });
        _add("jz", { rel16 }, { bits16, bits32, mode16 }, { 0x0F, 0x84 });

        _add("ja", { rel32 }, { all, mode32 }, { 0x0F, 0x87 });
        _add("jae", { rel32 }, { all, mode32 }, { 0x0F, 0x83 });
        _add("jb", { rel32 }, { all, mode32 }, { 0x0F, 0x82 });
        _add("jbe", { rel32 }, { all, mode32 }, { 0x0F, 0x86 });
        _add("jc", { rel32 }, { all, mode32 }, { 0x0F, 0x82 });
        _add("je", { rel32 }, { all, mode32 }, { 0x0F, 0x84 });
        _add("jg", { rel32 }, { all, mode32 }, { 0x0F, 0x8F });
        _add("jge", { rel32 }, { all, mode32 }, { 0x0F, 0x8D });
        _add("jl", { rel32 }, { all, mode32 }, { 0x0F, 0x8C });
        _add("jle", { rel32 }, { all, mode32 }, { 0x0F, 0x8E });
        _add("jna", { rel32 }, { all, mode32 }, { 0x0F, 0x86 });
        _add("jnae", { rel32 }, { all, mode32 }, { 0x0F, 0x82 });
        _add("jnb", { rel32 }, { all, mode32 }, { 0x0F, 0x83 });
        _add("jnbe", { rel32 }, { all, mode32 }, { 0x0F, 0x87 });
        _add("jnc", { rel32 }, { all, mode32 }, { 0x0F, 0x83 });
        _add("jne", { rel32 }, { all, mode32 }, { 0x0F, 0x85 });
        _add("jng", { rel32 }, { all, mode32 }, { 0x0F, 0x8E });
        _add("jnge", { rel32 }, { all, mode32 }, { 0x0F, 0x8C });
        _add("jnl", { rel32 }, { all, mode32 }, { 0x0F, 0x8D });
        _add("jnle", { rel32 }, { all, mode32 }, { 0x0F, 0x8F });
        _add("jno", { rel32 }, { all, mode32 }, { 0x0F, 0x81 });
        _add("jnp", { rel32 }, { all, mode32 }, { 0x0F, 0x8B });
        _add("jns", { rel32 }, { all, mode32 }, { 0x0F, 0x89 });
        _add("jnz", { rel32 }, { all, mode32 }, { 0x0F, 0x85 });
        _add("jo", { rel32 }, { all, mode32 }, { 0x0F, 0x80 });
        _add("jp", { rel32 }, { all, mode32 }, { 0x0F, 0x8A });
        _add("jpe", { rel32 }, { all, mode32 }, { 0x0F, 0x8A });
        _add("jpo", { rel32 }, { all, mode32 }, { 0x0F, 0x8B });
        _add("js", { rel32 }, { all, mode32 }, { 0x0F, 0x88 });
        _add("jz", { rel32 }, { all, mode32 }, { 0x0F, 0x84 });

        _add("jmp", { rel8 }, { all }, { 0xEB });
        _add("jmp", { rel16 }, { bits16, bits32, mode16 }, { 0xE9 });
        _add("jmp", { rel32 }, { all, mode32 }, { 0xE9 });
        _add("jmp", { rm16 }, { bits16, bits32, mode16 }, { 0xFF }, 0, 4, true);
        _add("jmp", { rm32 }, { all, mode32 }, { 0xFF }, 0, 4, true);
        _add("jmp", { rm64 }, { bits64 }, { 0xFF }, 0, 4, true);
        _add("jmp", { ptr16_16 }, { bits16, bits32, mode16 }, { 0xEA });
        _add("jmp", { ptr16_32 }, { bits16, bits32, mode32 }, { 0xEA });
        _add("jmp", { m16_16 }, { all, mode16 }, { 0xFF }, -1, 5, true);
        _add("jmp", { m16_32 }, { all, mode32 }, { 0xFF }, -1, 5, true);
        _add("jmp", { m16_64 }, { bits64, rexw }, { 0xFF }, -1, 5, true);

        _add("lea", { r16, m }, { all, mode16 }, { 0x8D }, 0, 1);
        _add("lea", { r32, m }, { all, mode32 }, { 0x8D }, 0, 1);
        _add("lea", { r64, m }, { bits64, rexw }, { 0x8D }, 0, 1);

        _add("leave", {}, { all }, { 0xC9 });

        _add("lfence", {}, { all }, { 0x0F, 0xAE }, -1, 5, true);

        _add("lgdt", { m16_32 }, { bits16, bits32 }, { 0x0F, 0x01 }, 0, 2, true);
        _add("lgdt", { m16_64 }, { bits64 }, { 0x0F, 0x01 }, 0, 2, true);
        _add("lidt", { m16_32 }, { bits16, bits32 }, { 0x0F, 0x01 }, 0, 3, true);
        _add("lidt", { m16_64 }, { bits64 }, { 0x0F, 0x01 }, 0, 3, true);
        _add("lldt", { rm16 }, { all }, { 0x0F, 0x00 }, 0, 2, true);

        _add("lock", {}, { all }, { 0xF0 });

        _add("lsl", { r16, r16m16 }, { all, mode16 }, { 0x0F, 0x03 }, 1, 0);
        _add("lsl", { r32, r32m16 }, { all, mode32 }, { 0x0F, 0x03 }, 1, 0);
        _add("lsl", { r64, r32m16 }, { bits64, rexw }, { 0x0F, 0x03 }, 1, 0);

        _add("ltr", { rm16 }, { all }, { 0x0F, 0x00 }, -1, 3, true);

        _add("mfence", {}, { all }, { 0x0F, 0xAE }, -1, 6, true);

        _add("mov", { rm8, r8 }, { all }, { 0x88 }, 0, 1);
        _add("mov", { rm8, r8 }, { bits64, rex }, { 0x88 }, 0, 1);
        _add("mov", { rm16, r16 }, { all, mode16 }, { 0x89 }, 0, 1);
        _add("mov", { rm32, r32 }, { all, mode32 }, { 0x89 }, 0, 1);
        _add("mov", { rm64, r64 }, { bits64, rexw }, { 0x89 }, 0, 1);
        _add("mov", { r8, rm8 }, { all }, { 0x8A }, 1, 0);
        _add("mov", { r8, rm8 }, { bits64, rex }, { 0x8A }, 1, 0);
        _add("mov", { r16, rm16 }, { all, mode16 }, { 0x8B }, 1, 0);
        _add("mov", { r32, rm32 }, { all, mode32 }, { 0x8B }, 1, 0);
        _add("mov", { r64, rm64 }, { bits64, rexw }, { 0x8B }, 1, 0);
        _add("mov", { rm16, sreg }, { all }, { 0x8C }, 0, 1);
        _add("mov", { rm32, sreg }, { all }, { 0x8C }, 0, 1);
        _add("mov", { rm64, sreg }, { bits64, rexw }, { 0x8C }, 0, 1);
        _add("mov", { sreg, rm16 }, { all }, { 0x8E }, 1, 0);
        _add("mov", { sreg, rm32 }, { all }, { 0x8E }, 1, 0);
        _add("mov", { sreg, rm64 }, { bits64, rexw }, { 0x8E }, 1, 0);
        _add("mov", { al, moffs8 }, { all }, { 0xA0 });
        _add("mov", { al, moffs8 }, { bits64, rexw }, { 0xA0 });
        _add("mov", { ax, moffs16 }, { all, mode16 }, { 0xA1 });
        _add("mov", { eax, moffs32 }, { all, mode32 }, { 0xA1 });
        _add("mov", { rax, moffs64 }, { bits64, rexw }, { 0xA1 });
        _add("mov", { moffs8, al }, { all }, { 0xA2 });
        _add("mov", { moffs8, al }, { bits64, rexw }, { 0xA2 });
        _add("mov", { moffs16, ax }, { all, mode16 }, { 0xA3 });
        _add("mov", { moffs32, eax }, { all, mode32 }, { 0xA3 });
        _add("mov", { moffs64, rax }, { bits64, rexw }, { 0xA3 });
        _add("mov", { r8, imm8 }, { all, rb }, { 0xB0 });
        _add("mov", { r8, imm8 }, { all, rb, rex }, { 0xB0 });
        _add("mov", { r16, imm16 }, { all, mode16, rw }, { 0xB8 });
        _add("mov", { r32, imm32 }, { all, mode32, rd }, { 0xB8 });
        _add("mov", { r64, imm64 }, { bits64, rexw, rd }, { 0xB8 });
        _add("mov", { rm8, imm8 }, { all }, { 0xC6 }, 0, 0, true);
        _add("mov", { rm8, imm8 }, { bits64, rexw }, { 0xC6 }, 0, 0, true);
        _add("mov", { rm16, imm16 }, { all, mode16 }, { 0xC7 }, 0, 0, true);
        _add("mov", { rm32, imm32 }, { all, mode32 }, { 0xC7 }, 0, 0, true);
        _add("mov", { rm64, imm32 }, { bits64, rexw }, { 0xC7 }, 0, 0, true);

        _add("mov", { r32, creg }, { bits16, bits32 }, { 0x0F, 0x20 }, 0, 1);
        _add("mov", { r64, creg }, { bits64 }, { 0x0F, 0x20 }, 0, 1);
        _add("mov", { r64, cr8 }, { bits64, rexr }, { 0x0F, 0x20 }, 0, 0, true);
        _add("mov", { creg, r32 }, { bits16, bits32 }, { 0xF0, 0x22 }, 1, 0);
        _add("mov", { creg, r64 }, { bits64 }, { 0xF0, 0x22 }, 1, 0);
        _add("mov", { cr8, r64 }, { bits64, rexr }, { 0xF0, 0x22 }, 1, 0, true);
        _add("mov", { r32, dreg }, { bits16, bits32 }, { 0xF0, 0x21 }, 0, 1);
        _add("mov", { r64, dreg }, { bits64 }, { 0xF0, 0x21 }, 0, 1);
        _add("mov", { dreg, r32 }, { bits16, bits32 }, { 0xF0, 0x23 }, 1, 0);
        _add("mov", { dreg, r64 }, { bits64 }, { 0xF0, 0x23 }, 1, 0);

        _add("movzx", { r16, rm8 }, { all, mode16 }, { 0x0F, 0xB6 }, 1, 0);
        _add("movzx", { r32, rm8 }, { all, mode32 }, { 0x0F, 0xB6 }, 1, 0);
        _add("movzx", { r64, rm8 }, { bits64, rexw }, { 0x0F, 0xB6 }, 1, 0);
        _add("movzx", { r32, rm16 }, { all }, { 0x0F, 0xB7 }, 1, 0);
        _add("movzx", { r64, rm16 }, { bits64, rexw }, { 0x0F, 0xB7 }, 1, 0);

        _add("mul", { rm8 }, { all }, { 0xF6 }, 0, 4, true);
        _add("mul", { rm8 }, { bits64, rex }, { 0xF6 }, 0, 4, true);
        _add("mul", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 4, true);
        _add("mul", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 4, true);
        _add("mul", { rm64 }, { bits64, rexw }, { 0xF7 }, 0, 4, true);

        _add("mul", { rm8 }, { all }, { 0xF6 }, 0, 3, true);
        _add("mul", { rm8 }, { bits64, rex }, { 0xF6 }, 0, 3, true);
        _add("mul", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 3, true);
        _add("mul", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 3, true);
        _add("mul", { rm64 }, { bits64, rexw }, { 0xF7 }, 0, 3, true);

        _add("nop", {}, { all }, { 0x90 });
        _add("nop", { rm16 }, { all, mode16 }, { 0x0F, 0x1F }, 0, 0, true);
        _add("nop", { rm32 }, { all, mode32 }, { 0x0F, 0x1F }, 0, 0, true);

        _add("mul", { rm8 }, { all }, { 0xF6 }, 0, 2, true);
        _add("mul", { rm8 }, { bits64, rex }, { 0xF6 }, 0, 2, true);
        _add("mul", { rm16 }, { all, mode16 }, { 0xF7 }, 0, 2, true);
        _add("mul", { rm32 }, { all, mode32 }, { 0xF7 }, 0, 2, true);
        _add("mul", { rm64 }, { bits64, rexw }, { 0xF7 }, 0, 2, true);

        _add("or", { al, imm8 }, { all }, { 0x0C });
        _add("or", { ax, imm16 }, { all, mode16 }, { 0x0D });
        _add("or", { eax, imm32 }, { all, mode32 }, { 0x0D });
        _add("or", { rax, imm64 }, { bits64, rexw }, { 0x0D });
        _add("or", { rm8, imm8 }, { all }, { 0x80 }, 0, 1, true);
        _add("or", { rm8, imm8 }, { bits64, rex }, { 0x80 }, 0, 1, true);
        _add("or", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 1, true);
        _add("or", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 1, true);
        _add("or", { rm64, imm32 }, { bits64, rexw }, { 0x81 }, 0, 1, true);
        _add("or", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 1, true);
        _add("or", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 1, true);
        _add("or", { rm64, imm8 }, { bits64, rex }, { 0x83 }, 0, 1, true);
        _add("or", { rm8, r8 }, { all }, { 0x08 }, 0, 1);
        _add("or", { rm8, r8 }, { bits64 }, { 0x08 }, 0, 1);
        _add("or", { rm16, r16 }, { all, mode16 }, { 0x09 }, 0, 1);
        _add("or", { rm32, r32 }, { all, mode32 }, { 0x09 }, 0, 1);
        _add("or", { rm64, r64 }, { bits64, rexw }, { 0x09 }, 0, 1);
        _add("or", { r8, rm8 }, { all }, { 0x0A }, 1, 0);
        _add("or", { r8, rm8 }, { bits64 }, { 0x0A }, 1, 0);
        _add("or", { r16, rm16 }, { all, mode16 }, { 0x0B }, 1, 0);
        _add("or", { r32, rm32 }, { all, mode32 }, { 0x0B }, 1, 0);
        _add("or", { r64, rm64 }, { bits64, rexw }, { 0x0B }, 1, 0);

        _add("out", { imm8, al }, { all }, { 0xE6 });
        _add("out", { imm8, ax }, { all, mode16 }, { 0xE7 });
        _add("out", { imm8, eax }, { all, mode32 }, { 0xE7 });
        _add("out", { dx, al }, { all }, { 0xEE });
        _add("out", { dx, ax }, { all, mode16 }, { 0xEF });
        _add("out", { dx, eax }, { all, mode32 }, { 0xEF });

        _add("pause", {}, { all }, { 0xF3, 0x90 });

        _add("pop", { rm16 }, { all, mode16 }, { 0x8F }, 0);
        _add("pop", { rm32 }, { bits16, bits32, mode32 }, { 0x8F }, 0);
        _add("pop", { rm64 }, { bits64 }, { 0x8F }, 0);
        _add("pop", { r16 }, { all, rw }, { 0x58 });
        _add("pop", { r32 }, { bits16, bits32, rd }, { 0x58 });
        _add("pop", { r64 }, { bits64, rd }, { 0x58 });
        _add("pop", { ds }, { bits16, bits32 }, { 0x1F });
        _add("pop", { es }, { bits16, bits32 }, { 0x07 });
        _add("pop", { ss }, { bits16, bits32 }, { 0x17 });
        _add("pop", { fs }, { bits64 }, { 0x0F, 0xA1 });
        _add("pop", { fs }, { all, mode16 }, { 0x0F, 0xA1 });
        _add("pop", { fs }, { bits16, bits32, mode32 }, { 0x0F, 0xA1 });
        _add("pop", { gs }, { bits64 }, { 0x0F, 0xA9 });
        _add("pop", { gs }, { all, mode16 }, { 0x0F, 0xA9 });
        _add("pop", { gs }, { bits16, bits32, mode32 }, { 0x0F, 0xA9 });

        _add("popa", {}, { bits16, bits32, mode16 }, { 0x61 });
        _add("popad", {}, { bits16, bits32, mode32 }, { 0x61 });

        _add("rdtsc", {}, { all }, { 0x0F, 0x31 });
        _add("rdtscp", {}, { all }, { 0x0F, 0x01, 0xF9 });

        _add("ret", {}, { all }, { 0xC3 });
        _add("retf", {}, { all }, { 0xCB });
        _add("ret", { imm16 }, { all }, { 0xC2 });
        _add("ret", { imm32 }, { all }, { 0xCA });

        _add("sfence", {}, { all }, { 0x0F, 0xAE }, -1, 7, true);

        _add("stc", {}, { all }, { 0xF9 });
        _add("std", {}, { all }, { 0xFD });
        _add("sti", {}, { all }, { 0xFB });

        _add("sub", { al, imm8 }, { all }, { 0x2C });
        _add("sub", { ax, imm16 }, { all, mode16 }, { 0x2D });
        _add("sub", { eax, imm32 }, { all, mode32 }, { 0x2D });
        _add("sub", { rax, imm64 }, { bits64, rexw }, { 0x2D });
        _add("sub", { rm8, imm8 }, { all }, { 0x80 }, 0, 5, true);
        _add("sub", { rm8, imm8 }, { bits64, rex }, { 0x80 }, 0, 5, true);
        _add("sub", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 5, true);
        _add("sub", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 5, true);
        _add("sub", { rm64, imm32 }, { bits64, rexw }, { 0x81 }, 0, 5, true);
        _add("sub", { rm16, imm8 }, { all, mode16 }, { 0x83 }, 0, 5, true);
        _add("sub", { rm32, imm8 }, { all, mode32 }, { 0x83 }, 0, 5, true);
        _add("sub", { rm64, imm8 }, { bits64, rex }, { 0x83 }, 0, 5, true);
        _add("sub", { rm8, r8 }, { all }, { 0x28 }, 0, 1);
        _add("sub", { rm8, r8 }, { bits64 }, { 0x28 }, 0, 1);
        _add("sub", { rm16, r16 }, { all, mode16 }, { 0x29 }, 0, 1);
        _add("sub", { rm32, r32 }, { all, mode32 }, { 0x29 }, 0, 1);
        _add("sub", { rm64, r64 }, { bits64, rexw }, { 0x29 }, 0, 1);
        _add("sub", { r8, rm8 }, { all }, { 0x2A }, 1, 0);
        _add("sub", { r8, rm8 }, { bits64 }, { 0x2A }, 1, 0);
        _add("sub", { r16, rm16 }, { all, mode16 }, { 0x2B }, 1, 0);
        _add("sub", { r32, rm32 }, { all, mode32 }, { 0x2B }, 1, 0);
        _add("sub", { r64, rm64 }, { bits64, rexw }, { 0x2B }, 1, 0);

        _add("syscall", {}, { bits64 }, { 0x0F, 0x05 });
        _add("sysenter", {}, { all }, { 0x0F, 0x34 });
        _add("sysexit", {}, { all }, { 0x0F, 0x35 });
        _add("sysexitd", {}, { all, rexw }, { 0x0F, 0x35 });
        _add("sysret", {}, { all }, { 0x0F, 0x07 });
        _add("sysretd", {}, { all, rexw }, { 0x0F, 0x07 });

        _add("test", { al, imm8 }, { all }, { 0xA8 });
        _add("test", { ax, imm16 }, { all, mode16 }, { 0xA9 });
        _add("test", { eax, imm32 }, { all, mode32 }, { 0xA9 });
        _add("test", { rax, imm32 }, { bits64, rexw }, { 0xA9 });
        _add("test", { rm8, imm8 }, { all }, { 0xF6 }, 0, 0, true);
        _add("test", { rm8, imm8 }, { bits64, rexw }, { 0xF6 }, 0, 0, true);
        _add("test", { rm16, imm16 }, { all, mode16 }, { 0xF7 }, 0, 0, true);
        _add("test", { rm32, imm32 }, { all, mode32 }, { 0xF7 }, 0, 0, true);
        _add("test", { rm64, imm32 }, { bits64, rexw }, { 0xF7 }, 0, 0, true);
        _add("test", { rm8, r8 }, { all }, { 0x84 }, 0, 1);
        _add("test", { rm8, r8 }, { bits64, rex }, { 0x84 }, 0, 1);
        _add("test", { rm16, r16 }, { all, mode16 }, { 0x85 }, 0, 1);
        _add("test", { rm32, r32 }, { all, mode32 }, { 0x85 }, 0, 1);
        _add("test", { rm64, r64 }, { bits64, rexw }, { 0x85 }, 0, 1);

        _add("xor", { al, imm8 }, { all }, { 0x34 });
        _add("xor", { ax, imm16 }, { all, mode16 }, { 0x35 });
        _add("xor", { eax, imm32 }, { all, mode32 }, { 0x35 });
        _add("xor", { rax, imm32 }, { bits64, rexw }, { 0x35 });
        _add("xor", { rm8, imm8 }, { all }, { 0x80 }, 0, 6, true);
        _add("xor", { rm8, imm8 }, { bits64, rex }, { 0x80 }, 0, 6, true);
        _add("xor", { rm16, imm16 }, { all, mode16 }, { 0x81 }, 0, 6, true);
        _add("xor", { rm32, imm32 }, { all, mode32 }, { 0x81 }, 0, 6, true);
        _add("xor", { rm64, imm32 }, { bits64, rexw }, { 0x81 }, 0, 6, true);
        _add("xor", { rm8, r8 }, { all }, { 0x30 }, 0, 1);
        _add("xor", { rm8, r8 }, { bits64, rex }, { 0x30 }, 0, 1);
        _add("xor", { rm16, r16 }, { all, mode16 }, { 0x31 }, 0, 1);
        _add("xor", { rm32, r32 }, { all, mode32 }, { 0x31 }, 0, 1);
        _add("xor", { rm64, r64 }, { bits64, rexw }, { 0x31 }, 0, 1);
        _add("xor", { r8, rm8 }, { all }, { 0x32 }, 1, 0);
        _add("xor", { r8, rm8 }, { bits64, rex }, { 0x32 }, 1, 0);
        _add("xor", { r16, rm16 }, { all, mode16 }, { 0x33 }, 1, 0);
        _add("xor", { r32, rm32 }, { all, mode32 }, { 0x33 }, 1, 0);
        _add("xor", { r64, rm64 }, { bits64, rexw }, { 0x33 }, 1, 0);
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
