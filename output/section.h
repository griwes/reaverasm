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
            section(const ast & a) : _ast{ a }
            {
            }

            void push(codepoint c)
            {
                _blob.emplace_back(std::move(c));
            }

            std::vector<uint8_t> blob() const
            {
                std::vector<uint8_t> ret;

                for (const auto & x : _blob)
                {
                    auto v = x.encode();
                    std::copy(v.begin(), v.end(), ret.end());
                }

                return ret;
            }

            std::vector<relocation> relocations() const
            {
                uint64_t offset = 0;
                std::vector<relocation> ret;

                for (const auto & x : _blob)
                {
                    if (!x.is_resolved())
                    {
                        ret.emplace_back(x.name(), _name, offset, _ast.is_local(x.name()));
                    }

                    offset += x.size();
                }

                return ret;
            }

        private:
            std::string _name;
            std::vector<codepoint> _blob;
            const ast & _ast;
        };
    }
}
