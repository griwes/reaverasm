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

#include <boost/program_options.hpp>

#include <reaver/target.h>

#include <frontend/frontend.h>

namespace reaver
{
    namespace assembler
    {
        class console_frontend : public frontend
        {
        public:
            console_frontend(int, char **);
            virtual ~console_frontend() {}

            virtual bool preprocess_only() const;

            virtual std::string preprocessor() const;
            virtual std::string syntax() const;
            virtual ::reaver::target::triple target() const;

        private:
            boost::program_options::variables_map _variables;
            bool _prep_only;
            bool _asm_only;

            std::ifstream _input;
            std::ofstream _output;

            ::reaver::target::triple _target;
        };
    }
}
