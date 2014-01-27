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

#pragma once

#include <string>
#include <set>
#include <vector>

#include <cpu/operand_matcher.h>
#include <cpu/codepoint.h>

namespace reaver
{
    namespace assembler
    {
        enum mode
        {
            bits16,
            bits32,
            bits64,
            mode16,
            mode32,
            rb,
            rw,
            rd,
            ro,
            rex,
            rexw,
            rexr,
            all
        };

        extern operand_type rel8;
        extern operand_type rel16;
        extern operand_type rel32;
        extern operand_type ptr16_16;
        extern operand_type ptr16_32;
        extern operand_type r8;
        extern operand_type r16;
        extern operand_type r32;
        extern operand_type r64;
        extern operand_type imm8;
        extern operand_type imm16;
        extern operand_type imm32;
        extern operand_type imm64;
        extern operand_type rm8;
        extern operand_type rm16;
        extern operand_type rm32;
        extern operand_type rm64;
        extern operand_type m;
        extern operand_type m8;
        extern operand_type m16;
        extern operand_type m32;
        extern operand_type m64;
        extern operand_type m128;
        extern operand_type m16_16;
        extern operand_type m16_32;
        extern operand_type m16_64;
        extern operand_type moffs8;
        extern operand_type moffs16;
        extern operand_type moffs32;
        extern operand_type moffs64;
        extern operand_type sreg;
        extern operand_type creg;
        extern operand_type dreg;
        extern operand_type cr8;
        extern operand_type al;
        extern operand_type ax;
        extern operand_type eax;
        extern operand_type rax;
        extern operand_type dx;
        extern operand_type r16m16;
        extern operand_type r32m16;
        extern operand_type ds;
        extern operand_type es;
        extern operand_type fs;
        extern operand_type gs;
        extern operand_type ss;

        class opcode
        {
        public:
            opcode(std::string mnemonic, std::vector<operand_type> && operands, std::set<mode> && mode, std::vector<codepoint> && code,
                int8_t rm_index, int8_t reg_index, bool special_reg) : _mnemonic{ mnemonic }, _operands{ std::move(operands) },
                _mode{ std::move(mode) }, _code{ std::move(code) }, _rm_index{ rm_index }, _reg_index{ reg_index },
                _special_reg{ special_reg }
            {
            }

            std::string mnemonic() const
            {
                return _mnemonic;
            }

            const std::vector<operand_type> & operands() const
            {
                return _operands;
            }

            const std::set<mode> & mode() const
            {
                return _mode;
            }

            const std::vector<codepoint> & code() const
            {
                return _code;
            }

            int8_t rm_index() const
            {
                return _rm_index;
            }

            int8_t reg_index() const
            {
                return _reg_index;
            }

            bool special_reg() const
            {
                return _special_reg;
            }

        private:
            std::string _mnemonic;
            std::vector<operand_type> _operands;
            std::set<reaver::assembler::mode> _mode;
            std::vector<codepoint> _code;
            int8_t _rm_index;
            int8_t _reg_index;
            bool _special_reg;
        };
    }
}
