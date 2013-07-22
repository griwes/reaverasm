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

#include <frontend.h>
#include <cpu/instruction.h>

namespace reaver
{
    namespace assembler
    {
        class section;

        class ast
        {
        public:
            void add_label(std::string name)
            {
                _labels[_lines.size()] = std::move(name);
            }

            void add_global(std::string name)
            {
                _globals.emplace(std::move(name));
            }

            void add_extern(std::string name)
            {
                _externs.emplace(std::move(name));
            }

            void start_section(std::string name)
            {
                _sections[_lines.size()] = std::move(name);
            }

            void set_bitness(uint64_t bitness)
            {
                _bitness_changes[_lines.size()] = bitness;
            }

            void add_instruction(const instruction & instr)
            {
                _lines.emplace_back(instr);
            }

            void add_data(const data & d)
            {
                _lines.emplace_back(d);
            }

            bool is_local(const std::string & name) const
            {
                if (std::find_if(_labels.begin(), _labels.end(), [&](const std::pair<uint64_t, std::string> & v)
                    {
                        return v.second == name;
                    }) != _labels.end())
                {
                    return true;
                }

                else if (std::find(_externs.begin(), _externs.end(), name) != _externs.end())
                {
                    return false;
                }

                throw "unknown symbol used.";
            }

            const std::set<std::string> & globals() const
            {
                return _globals;
            }

            const std::set<std::string> & externs() const
            {
                return _externs;
            }

            std::map<std::string, section> assemble(const frontend &) const;

        private:
            // TODO: include_chain!!!
            std::vector<boost::variant<instruction, data>> _lines;
            std::map<uint64_t, std::string> _labels;
            std::set<std::string> _globals;
            std::set<std::string> _externs;
            std::map<uint64_t, std::string> _sections;
            std::map<uint64_t, uint64_t> _bitness_changes;
        };
    }
}
