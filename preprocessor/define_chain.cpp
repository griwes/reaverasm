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

#include "define_chain.h"

reaver::exception reaver::assembler::define_chain::create_exception() const
{
    throw exception(logger::crash) << "not implemented yet: " << __PRETTY_FUNCTION__;
}

void reaver::assembler::define_chain::push(uint64_t start, uint64_t end, uint64_t length, std::shared_ptr<define> def)
{
    for (auto & expansion : _define_expansions)
    {
        int64_t diff = length - (end - start);

        // expansion:   [==================]
        // new:             [=========]
        if (expansion.start <= start && expansion.end >= end)
        {
            expansion.end += diff;
        }

        // expansion:   [==========]
        // new:             [==========]
        else if (expansion.start <= start && expansion.end < end)
        {
            // this is possibly a tiny bit less broken than the previous version
            expansion.end = end;
        }
    }

    _define_expansions.emplace_back(start, start + length, def);
}
