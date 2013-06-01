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

namespace reaver
{
    namespace assembler
    {
        class relocation
        {
        public:
            relocation(std::string symbol_name, uint64_t offset, std::string section, int64_t addend, bool relative) :
                section{ std::move(section) }, offset{ offset }, symbol{ std::move(symbol_name) }, addend{ addend },
                pc_relative{ relative }
            {
            }

            std::string section;
            uint64_t offset;
            std::string symbol;
            int64_t addend;
            bool pc_relative;
        };
    }
}
