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
#include <fstream>

#include <boost/filesystem.hpp>

#include <preprocessor/preprocessor.h>
#include <assembler.h>

reaver::assembler::preprocessor::preprocessor(const std::map<std::string, std::shared_ptr<reaver::assembler::macro>> & macros)
    : _macros(macros)
{
}

std::vector<reaver::assembler::line> reaver::assembler::preprocessor::preprocess(const std::string & _buffer, std::string name)
{
    std::stringstream input(_buffer);
    std::vector<std::pair<std::string, uint64_t>> include_chain{ std::make_pair(boost::filesystem::absolute(name).string(), 0) };

    _include_stream(input, include_chain);

    if (_error)
    {
        std::exit(-1);
    }

    for (auto & x : _lines)
    {
        std::cout << *x << std::endl;
    }

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
        }

        else if (buffer.find('%') == 0)
        {
            if (buffer.find("%include") == 0)
            {
                auto opening = buffer.find('"');
                auto closing = buffer.find('"', opening + 1);

                for (uint64_t i = closing + 1; i < buffer.size(); ++i)
                {
                    if (!std::isspace(buffer[i]))
                    {
                        _print_include_chain(include_chain);
                        std::cout << "Error: junk after %include directive.\n";

                        _error = true;
                    }
                }

                if (_error)
                {
                    continue;
                }

                std::string filename = buffer.substr(opening + 1, closing - opening - 1);
                std::fstream in;

                if (boost::filesystem::is_regular_file(filename))
                {
                    in.open(filename);
                }

                else if (boost::filesystem::is_regular_file(boost::filesystem::path(include_chain.back().first).parent_path().string() + filename))
                {
                    filename = boost::filesystem::path(include_chain.back().first).parent_path().string() + filename;
                    in.open(filename);
                }

                // TODO: -I from frontend

                else
                {
                    _print_include_chain(include_chain);
                    std::cout << "Error: included file " << filename << " not found.\n";

                    _error = true;
                }

                if (!_error)
                {
                    if (!in)
                    {
                        _print_include_chain(include_chain);
                        std::cout << "Error: failed to open included file " << filename << ".\n";

                        _error = true;
                    }

                    else
                    {
                        auto new_include_chain = include_chain;
                        new_include_chain.emplace_back(std::make_pair(filename, 0));

                        assembler as;
                        as.read_input(in, filename);
                        buffer = as.buffer();
                        std::stringstream sin{buffer};

                        _include_stream(sin, new_include_chain);
                    }
                }
            }
        }
    }
}

void reaver::assembler::preprocessor::_print_include_chain(const std::vector<std::pair<std::string, uint64_t>> & include_chain) const
{
    std::cout << "In file " << include_chain.back().first << " in line " << include_chain.back().second << ":\n";

    for (auto it = include_chain.crbegin() + 1; it != include_chain.crend(); ++it)
    {
        if (it->second != (uint64_t)-1)
        {
            std::cout << "Included from file " << it->first << " in line " << it->second << ":\n";
        }

        else
        {
            std::cout << "In expanded macro `" << it->first << "`:\n";
        }
    }
}
