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

#include <string>
#include <sstream>

#include <preprocessor/preprocessor.h>

reaver::assembler::preprocessor::preprocessor(const std::map<std::string, std::shared_ptr<reaver::assembler::macro>> & macros)
    : _macros(macros)
{
}

std::vector<reaver::assembler::line> reaver::assembler::preprocessor::preprocess(const std::string & _buffer, std::string name)
{
    std::stringstream input(_buffer);
    std::vector<std::pair<std::string, uint64_t>> include_chain{ std::make_pair(name, 0) };

    _include_stream(input, include_chain);

    return _lines;
}

void reaver::assembler::preprocessor::_include_stream(std::istream & input, std::vector<std::pair<std::string, uint64_t>> include_chain)
{
    std::string buffer;

    while (std::getline(input, buffer) && ++include_chain.back().second)
    {
        if (buffer.find("%") == std::string::npos)
        {
            _lines.emplace_back(buffer, include_chain);

            continue;
        }
    }
}
