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
#include <vector>

#include <cpu/codepoint.h>
#include <output/relocation.h>
#include <parser/ast.h>

namespace reaver
{
    namespace assembler
    {
        class section
        {
        public:
            section(std::string name, const ast & a) : _name{ std::move(name) }, _ast{ a }
            {
            }

            section(std::string name) : _name{ std::move(name) }
            {
            }

            void push(codepoint c)
            {
                _offset += c.size();
                _blob.emplace_back(std::move(c));
            }

            void push(const std::string & symbol)
            {
                _symbols[symbol] = _offset;
            }

            std::vector<uint8_t> blob() const
            {
                std::vector<uint8_t> ret;

                for (const auto & x : _blob)
                {
                    auto v = x.encode();
                    std::copy(v.begin(), v.end(), std::back_inserter(ret));
                }

                return ret;
            }

            std::vector<relocation> relocations() const
            {
                if (!_ast)
                {
                    throw "invalid relocations() call on section detached from ast, consider this an internal error.";
                }

                uint64_t offset = 0;
                std::vector<relocation> ret;

                for (const auto & x : _blob)
                {
                    if (!x.is_resolved())
                    {
                        // this here is a silly hack that will have to go away one day...
                        ret.emplace_back(x.name(), offset, _name, _ast->is_local(x.name()) ? 0 : -4, _ast->is_local(x.name()));
                    }

                    offset += x.size();
                }

                return ret;
            }

        private:
            std::string _name;
            std::vector<codepoint> _blob;
            std::map<std::string, uint64_t> _symbols;
            boost::optional<const ast &> _ast;
            uint64_t _offset = 0;
        };
    }
}
