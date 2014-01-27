/**
 * Reaver Project Assembler License
 *
 * Copyright © 2013-2014 Michał "Griwes" Dominiak
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

#include "../preprocessor.h"

namespace reaver
{
    namespace assembler
    {
        class none_preprocessor : public preprocessor
        {
        public:
            none_preprocessor(const frontend & front) : _front{ front }
            {
            }

            virtual ~none_preprocessor() {}

            virtual std::vector<line> operator()() const override
            {
                // the following return is slowly turning into brace abomination... God bless C++11 uniform initialization
                // - if it wasn't available, it would be an even worse monstrocity that would have to mention every type
                // name used in this invocation, which would be even more horrible. brr.
                return { { { { std::istreambuf_iterator<char>{ _front.input().rdbuf() }, std::istreambuf_iterator<char>{} },
                    {} }, {}, 0, {} } };
            }

        private:
            const frontend & _front;
        };
    }
}
