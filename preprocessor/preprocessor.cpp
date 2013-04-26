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

#include <preprocessor/preprocessor.h>
#include <preprocessor/parser.h>
#include <utils.h>

reaver::assembler::preprocessor::preprocessor(reaver::assembler::frontend & front) : _frontend{ front }
{
}

std::vector<reaver::assembler::line> reaver::assembler::preprocessor::preprocess(const std::string & _buffer)
{
    if (_frontend.default_includes())
    {
        for (auto & file : _frontend.get_default_includes())
        {
            std::stringstream input{ _frontend.read_file(file, { { std::string("<command line>"), 0, false } }).first };

            _include_stream(input, { { std::string("<command line>"), 0, false }, { file, 0, false } });
        }
    }

    std::stringstream input(_buffer);
    include_chain include_chain{ { _frontend.absolute_name(), 0, false } };

    _include_stream(input, include_chain);

    for (auto & x : _lines)
    {
        std::cout << *x << std::endl;
    }

    return _lines;
}

void reaver::assembler::preprocessor::_include_stream(std::istream & input, include_chain include_chain)
{
    std::string buffer;
    uint64_t new_lines;

    while (std::getline(input, buffer) && (include_chain.back().line += new_lines))
    {
        new_lines = 1;
        std::vector<std::string> v{ buffer };

        if (include_chain.size() > 128)
        {
            print_include_chain(include_chain);
            dlog(error) << "maximum include (or macro) depth (128) reached.";

            std::exit(-1);
        }

        while (buffer.size() && buffer.back() == '\\')
        {
            buffer.pop_back();

            std::string next;

            if (!std::getline(input, next))
            {
                print_include_chain(include_chain);
                dlog(error) << "invalid `\\` at the end of file.";

                std::exit(-1);
            }

            buffer.append(next);
            v.push_back(next);
            ++new_lines;
        }

        preprocessor_lexer lexer;
        auto tokens = reaver::lexer::tokenize(buffer, lexer.desc);

        preprocessor_parser parser{ lexer };

        auto begin = tokens.cbegin();

        {
            auto define = parser.define.match(begin, tokens.cend(), parser.skip);

            if (define)
            {
                dlog() << "Define matched; name: " << define->name;
            }
        }
    }
}