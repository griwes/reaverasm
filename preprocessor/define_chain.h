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

#include <ostream>
#include <vector>

#include <reaver/logger.h>
#include <reaver/exception.h>

#include <preprocessor/define.h>

namespace reaver
{
    namespace assembler
    {
        class define;

        struct define_info
        {
            define_info(uint64_t s, uint64_t e, std::shared_ptr<define> def) : start{ s }, end{ e }, define{ std::move(def) }
            {
            }

            uint64_t start;
            uint64_t end;
            std::shared_ptr<define> define;
        };

        class define_chain
        {
        public:
            define_chain() = default;
            define_chain(const define_chain &) = default;

            operator bool() const
            {
                return !_define_expansions.empty();
            }

            void push(uint64_t, uint64_t, uint64_t, std::shared_ptr<define>);

            void print(logger::logger &) const;

        private:
            std::vector<define_info> _define_expansions;
        };

        inline logger::logger & operator<<(logger::logger & l, const define_chain & chain)
        {
            chain.print(l);
            return l;
        }

        inline std::ostream & operator<<(std::ostream & stream, const define_chain & chain)
        {
            std::stringstream ss;
            logger::logger log{ ss };
            log << chain;
            return stream << ss.str();
        }
    }
}
