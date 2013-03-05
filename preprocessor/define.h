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
#include <string>
#include <utility>

namespace reaver
{
    namespace assembler
    {
        class define
        {
        public:
            define()
            {
            }

            define(std::string name, std::string definition, std::vector<std::pair<std::string, uint64_t>> include_chain)
                : _name(name), _body(definition), _inc_chain(include_chain)
            {
            }

            define(std::string name, std::vector<std::string> params, std::string definition,
                std::vector<std::pair<std::string, uint64_t>> include_chain) : _name(name), _body(definition), _inc_chain(include_chain),
                _params(params)
            {
            }

            std::string name()
            {
                return _name;
            }

            std::string definition()
            {
                return _body;
            }

            std::vector<std::pair<std::string, uint64_t>> source()
            {
                return _inc_chain;
            }

            std::vector<std::string> parameters()
            {
                return _params;
            }

        private:
            std::string _name;
            std::string _body;
            std::vector<std::pair<std::string, uint64_t>> _inc_chain;
            std::vector<std::string> _params;
        };
    }
}
