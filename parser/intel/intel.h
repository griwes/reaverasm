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

#include <reaver/error.h>

#include "../parser.h"
#include "../../utils/include_chain.h"

namespace reaver
{
    namespace assembler
    {
        class intel_parser : public parser
        {
        public:
            intel_parser(const frontend & front, error_engine & engine) : _front{ front }, _engine{ engine }
            {
            }

            virtual ~intel_parser() {}

            virtual ast operator()() const override;

        private:
            const frontend & _front;
            error_engine & _engine;

            ast _parse_stream(std::istream &, std::shared_ptr<utils::include_chain>) const;
        };
    }
}
