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

#include <vector>
#include <memory>
#include <map>

#include <frontend.h>
#include <preprocessor/macro.h>
#include <preprocessor/line.h>
#include <preprocessor/define.h>

namespace reaver
{
    namespace assembler
    {
        class preprocessor
        {
        public:
            preprocessor(frontend &);

            std::vector<line> preprocess(const std::string &);

        private:
            void _include_stream(std::istream &, std::vector<std::pair<std::string, uint64_t>>);
            bool _valid_macro_name(const std::string &) const;
            std::vector<std::string> _tokenize(const std::string &) const;       // PPC tokenization is kinda lame...

            std::map<std::string, define> _defines;
            std::map<std::string, std::shared_ptr<macro>> _macros;
            std::vector<line> _lines;

            frontend & _frontend;
        };
    }
}
