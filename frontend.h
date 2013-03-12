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

#include <fstream>
#include <string>
#include <utility>

#include <boost/program_options.hpp>

#include <preprocessor/define.h>

namespace reaver
{
    namespace assembler
    {
        class frontend
        {
        public:
            frontend(int, char **);

            std::ostream & output() const;

            std::string input_name() const;
            std::string absolute_name() const;

            std::map<std::string, define> defines();

            std::string read_file() const;
            std::pair<std::string, std::string> read_file(std::string filename, std::vector<std::pair<std::string, uint64_t>>) const;

            bool default_includes() const;
            std::vector<std::string> get_default_includes() const;

            bool preprocess_only() const;

        private:
            std::string _resolve_name(std::string, std::vector<std::pair<std::string, uint64_t>>) const;

            boost::program_options::variables_map _variables;

            bool _prep_only = false;
            bool _asm_only = false;

            mutable std::fstream _output;
        };
    }
}
