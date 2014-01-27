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

#include <memory>
#include <map>

#include <reaver/error.h>
#include <reaver/format/executable.h>

#include "../frontend/frontend.h"
#include "../parser/ast.h"

namespace reaver
{
    namespace assembler
    {
        class generator
        {
        public:
            generator()
            {
            }

            virtual ~generator() {}

            virtual std::unique_ptr<format::executable::executable> operator()(const ast &) const = 0;
        };

        std::unique_ptr<generator> create_generator(const frontend &, error_engine &);
    }
}
