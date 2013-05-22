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

#include <vector>
#include <map>

#include <cpu/instruction.h>

namespace reaver
{
    namespace assembler
    {
        class ast
        {
        public:
            void add_label(std::string);
            void add_global(std::string);
            void add_extern(std::string);
            void start_section(std::string);
            void set_bitness(uint64_t);
            void add_instruction(const instruction &);
            void add_data(const data &);

        private:
            std::vector<boost::variant<instruction, data>> _line;
            std::map<std::string, uint64_t> _labels; // label name -> instruction index
            std::vector<std::string> _globals;
            std::vector<std::string> _externs;
            std::map<uint64_t, std::string> _sections;
            std::map<uint64_t, uint64_t> _bitness_changes;
        };
    }
}