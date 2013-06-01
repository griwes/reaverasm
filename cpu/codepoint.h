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

namespace reaver
{
    namespace assembler
    {
        class codepoint
        {
        public:
            codepoint(uint8_t code) : _code{ code }
            {
            }

            codepoint(std::string name, uint64_t size) : _code{ size }, _name{ std::move(name) }
            {
            }

            bool is_resolved() const
            {
                return _name.empty();
            }

            uint64_t size() const
            {
                return _name.empty() ? 1 : _code / 8;
            }

            std::string name() const
            {
                return _name;
            }

            uint8_t code() const
            {
                if (_code > 255)
                {
                    throw "called code() on unresolved codepoint, consider this internal error.";
                }

                return static_cast<uint8_t>(_code);
            }

            std::vector<uint8_t> encode() const
            {
                if (_name.empty())
                {
                    return { code() };
                }

                return std::vector<uint8_t>(_code / 8);
            }

        private:
            uint64_t _code;
            std::string _name;
        };
    }
}
