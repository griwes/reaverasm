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

#include <memory>

#include <reaver/exception.h>

namespace reaver
{
    namespace assembler
    {
        namespace utils
        {
            struct include_chain
            {
                include_chain(const include_chain &) = default;
                include_chain(include_chain &&) = default;

                include_chain(std::string f = "", std::shared_ptr<include_chain> u = nullptr, uint64_t l = 0, bool m = false)
                    : file{ std::move(f) }, up{ std::move(u) }, line{ l }, macro{ m }
                {
                    if (up)
                    {
                        size = up->size + 1;
                    }
                }

                class exception exception(bool b = false) const
                {
                    if (up)
                    {
                        return up->exception(true) << "\nIn file " << (b ? "included from " : "") << file << " at line "
                            << line << ":";
                    }

                    return reaver::exception(logger::always) << "In file " << (b ? "included from " : "") << file << " at line "
                        << line << ":";
                }

                std::string file;
                std::shared_ptr<include_chain> up;
                uint64_t line;
                bool macro;

                uint64_t size = 0;
            };
        }
    }
}
