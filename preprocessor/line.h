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

#include <iostream>
#include <string>
#include <vector>

#include <reaver/logger.h>

namespace reaver
{
    namespace assembler
    {
        class line
        {
        public:
            line(std::string line, std::vector<std::pair<std::string, uint64_t>> chain) : _line(line), _include_chain(chain)
            {
            }

            std::string operator*()
            {
                return _line;
            }

            uint64_t include_count()
            {
                return _include_chain.size();
            }

            std::pair<std::string, uint64_t> operator[](uint64_t i)
            {
                if (i >= _include_chain.size())
                {
                    reaver::logger::log(reaver::logger::crash) << "include chain access out of bounds.";

                    std::exit(-2);
                }

                return _include_chain[i];
            }

        private:
            std::string _line;
            std::vector<std::pair<std::string, uint64_t>> _include_chain;
        };
    }
}
