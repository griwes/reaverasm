/**
 * Reaver Project Assembler License
 *
 * Copyright © 2013 Michał "Griwes" Dominiak
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
 **/

#pragma once

#include <string>
#include <vector>

#include "../utils/include_chain.h"
#include "define_chain.h"

namespace reaver
{
    namespace assembler
    {
        struct line
        {
            line(std::pair<std::string, class define_chain> pp, std::vector<std::string> orig, uint64_t number, std::shared_ptr<
                utils::include_chain> inc) : preprocessed{ std::move(pp.first) }, original{ std::move(orig) }, number{ number },
                include_chain{ std::move(inc) }, define_chain{ std::move(pp.second) }
            {
            }

            std::string preprocessed;
            std::vector<std::string> original;
            uint64_t number;

            std::shared_ptr<utils::include_chain> include_chain;
            class define_chain define_chain;
        };
    }
}
