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

        enum operand_type
        {
            rel8,
            rel16,
            rel32,
            ptr16_16,
            ptr16_32,
            r8,
            r16,
            r32,
            r64,
            imm8,
            imm16,
            imm32,
            imm64,
            rm8,
            rm16,
            rm32,
            rm64,
            m,
            m8,
            m16,
            m32,
            m64,
            m128,
            m16_16,
            m16_32,
            m16_64,
            moffs8,
            moffs16,
            moffs32,
            moffs64,
            sreg,
            creg,
            dreg,
            cr8,
            al,
            ax,
            eax,
            rax,
            dx,
            r16m16,
            r32m16,
            ds,
            es,
            fs,
            gs,
            ss
        };

        class opcode
        {
        public:
            opcode(std::string mnemonic, std::vector<operand_type> && operands, std::set<mode> && mode, std::vector<uint8_t> && code,
                int8_t rm_index, int8_t reg_index, bool special_reg) : _mnemonic{ mnemonic }, _operands{ std::move(operands) },
                _mode{ std::move(mode) }, _code{ std::move(code) }, _rm_index{ rm_index }, _reg_index{ reg_index },
                _special_reg{ special_reg}
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

            const std::vector<uint8_t> & code() const
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
            std::vector<uint8_t> _code;
            int8_t _rm_index;
            int8_t _reg_index;
            bool _special_reg;
        };
    }
}
