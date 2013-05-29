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

#include <cpu/opcode.h>

namespace reaver
{
    namespace assembler
    {
        using cpu_register::sizes::byte;
        using cpu_register::sizes::word;
        using cpu_register::sizes::dword;
        using cpu_register::sizes::qword;
        using cpu_register::types::segment;
        using cpu_register::types::control;
        using cpu_register::types::debug;

        operand_type rel8 = immediate_value_matcher{ byte };
        operand_type rel16 = immediate_value_matcher{ word };
        operand_type rel32 = immediate_value_matcher{ dword };
        operand_type ptr16_16 = far_pointer_matcher{};
        operand_type ptr16_32 = far_pointer_matcher{};
        operand_type r8 = register_matcher{ byte };
        operand_type r16 = register_matcher{ word };
        operand_type r32 = register_matcher{ dword };
        operand_type r64 = register_matcher{ qword };
        operand_type imm8 = immediate_value_matcher{ byte };
        operand_type imm16 = immediate_value_matcher{ word };
        operand_type imm32 = immediate_value_matcher{ dword };
        operand_type imm64 = immediate_value_matcher{ qword };
        operand_type rm8 = rm_matcher{ byte };
        operand_type rm16 = rm_matcher{ word };
        operand_type rm32 = rm_matcher{ dword };
        operand_type rm64 = rm_matcher{ qword };
        operand_type m = address_matcher{};
//        operand_type m8;
//        operand_type m16;
//        operand_type m32;
//        operand_type m64;
//        operand_type m128;
        operand_type m16_16 = far_pointer_matcher{};
        operand_type m16_32 = far_pointer_matcher{};
        operand_type m16_64 = far_pointer_matcher{};
/*        operand_type moffs8 = moffs_matcher{};
        operand_type moffs16 = moffs_matcher{};
        operand_type moffs32 = moffs_matcher{};
        operand_type moffs64 = moffs_matcher{};
*/        operand_type sreg = special_register_matcher{ segment };
        operand_type creg = special_register_matcher{ control };
        operand_type dreg = special_register_matcher{ debug };
        operand_type cr8 = register_matcher{ "cr8" };
        operand_type al = register_matcher{ "al" };
        operand_type ax = register_matcher{ "ax" };
        operand_type eax = register_matcher{ "eax" };
        operand_type rax = register_matcher{ "rax" };
        operand_type dx = register_matcher{ "dx" };
        operand_type r16m16 = rm_matcher{ word };
        operand_type r32m16 = rm_matcher{ byte };
        operand_type ds = register_matcher{ "ds" };
        operand_type es = register_matcher{ "es" };
        operand_type fs = register_matcher{ "fs" };
        operand_type gs = register_matcher{ "gs" };
        operand_type ss = register_matcher{ "ss" };
    }
}
