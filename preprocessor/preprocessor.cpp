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

#include <utils.h>
#include <assembler.h>
#include <preprocessor/preprocessor.h>
#include <preprocessor/macro.h>

reaver::assembler::preprocessor::preprocessor(reaver::assembler::frontend & front) : _defines(front.defines()), _frontend(front)
{
}

std::vector<reaver::assembler::line> reaver::assembler::preprocessor::preprocess(const std::string & _buffer)
{
    if (_frontend.default_includes())
    {
        for (auto & file : _frontend.get_default_includes())
        {
            std::stringstream input(_frontend.read_file(file, { std::make_pair(std::string("<command line>"), 0) }));

            _include_stream(input, { std::make_pair(std::string("<command line>"), 0), std::make_pair(file, 0) });
        }
    }

    std::stringstream input(_buffer);
    std::vector<std::pair<std::string, uint64_t>> include_chain{ std::make_pair(_frontend.absolute_name(), 0) };

    _include_stream(input, include_chain);

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
        if (buffer.back() == '\\')
        {
            print_include_chain(include_chain);
            std::cout << "Error: invalid `\\` at the end of line.\n";

            std::exit(-1);
        }

        if (buffer.find("%") == std::string::npos)
        {
            auto tokenized = _tokenize(buffer);
            buffer.clear();

            for (auto & x : tokenized)
            {
                if (_defines.find(x) == _defines.end())
                {
                    buffer.append(x);
                }

                else
                {
                    define macro = _defines[x];

                    if (macro.parameters().size())
                    {

                    }

                    else
                    {
                        buffer.append(macro.definition());
                    }
                }
            }

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
                        print_include_chain(include_chain);
                        std::cout << "Error: junk after %include directive.\n";

                        std::exit(-1);
                    }
                }

                std::string filename = buffer.substr(opening + 1, closing - opening - 1);
                std::string include = _frontend.read_file(filename, include_chain);

                auto new_include_chain = include_chain;
                new_include_chain.emplace_back(std::make_pair(filename, 0));

                std::stringstream sin{include};

                _include_stream(sin, new_include_chain);
            }

            else if (buffer.find("%define") == 0)
            {
                std::vector<std::string> tokenized = _tokenize(remove_leading_whitespace(buffer.substr(7)));

                std::string name = tokenized[0];
                std::string definition;

                if (!_valid_macro_name(name))
                {
                    print_include_chain(include_chain);
                    std::cout << "Error: invalid macro name `" << name << "`.\n";

                    std::exit(-1);
                }

                if (_defines.find(name) != _defines.end())
                {
                    print_include_chain(include_chain);
                    std::cout << "Error: macro name `" << name << "` redefined.\n";
                    print_include_chain(_defines[name].source());
                    std::cout << "Note: first defined here.\n";

                    std::exit(-1);
                }

                if (tokenized[1] == "(")
                {
                    std::vector<std::string> params;

                    auto it = tokenized.begin() + 2;
                    for (; *it != ")"; ++it)
                    {
                        if (it == tokenized.end())
                        {
                            print_include_chain(include_chain);
                            std::cout << "Error: macro parameter list not closed.\n";

                            std::exit(-1);
                        }

                        if (*it == "," || remove_leading_whitespace(*it) == std::string())
                        {
                        }

                        else if (_valid_macro_name(*it))
                        {
                            params.push_back(*it);
                        }

                        else
                        {
                            print_include_chain(include_chain);
                            std::cout << "Error: invalid element in macro parameter list: `" << *it << "`.\n";

                            std::exit(-1);
                        }
                    }

                    ++it;

                    for (; it != tokenized.end(); ++it)
                    {
                        definition += *it;
                    }

                    _defines.emplace(name, define(name, params, definition, include_chain));
                }

                else
                {
                    for (auto it = tokenized.begin() + 1; it != tokenized.end(); ++it)
                    {
                        definition += *it;
                    }

                    _defines.emplace(name, define(name, definition, include_chain));
                }
            }

            else if (buffer.find("%xdefine") != std::string::npos)
            {

            }

            else if (buffer.find("%assign") != std::string::npos)
            {

            }

            else if (buffer.find("%undef") != std::string::npos)
            {

            }

            else if (buffer.find("%assign") != std::string::npos)
            {

            }

            else if (buffer.find("%strlen") != std::string::npos)
            {

            }

            else if (buffer.find("%substr") != std::string::npos)
            {

            }

            else if (buffer.find("%macro") != std::string::npos)
            {

            }

            else if (buffer.find("%if") != std::string::npos)
            {

            }

            else if (buffer.find("%error") != std::string::npos)
            {
                print_include_chain(include_chain);
                std::cout << "User defined error: '" << buffer.substr(7) << "'.\n";

                std::exit(-1);
            }

            else if (buffer.find("%rep") != std::string::npos)
            {

            }

            else if (buffer.find("%push") != std::string::npos)
            {

            }

            else if (buffer.find("%pop") != std::string::npos)
            {

            }
        }
    }
}

bool reaver::assembler::preprocessor::_valid_macro_name(const std::string & name) const
{
    if (!std::isalpha(name[0]) && name[0] != '_')
    {
        return false;
    }

    for (auto it = name.begin() + 1; it != name.end(); ++it)
    {
        if (!std::isalnum(name[0]) && name[0] != '_')
        {
            return false;
        }
    }

    return true;
}

std::vector<std::string> reaver::assembler::preprocessor::_tokenize(const std::string & input) const
{
    std::vector<std::string> ret;
    std::string current;
    current.reserve(256);

    for (auto it = input.begin(); it != input.end(); ++it)
    {
        if (std::isspace(*it))
        {
            if (current.size() == 0)
            {
                if (ret.back() == std::string{' '})
                {
                    continue;
                }

                ret.push_back({' '});
            }

            else
            {
                ret.push_back(current);
                ret.push_back({' '});

                current.clear();
            }

            continue;
        }

        if (*it == ',' || *it == '(' || *it == ')')
        {
            ret.push_back(current);
            current = *it;

            continue;
        }

        current.push_back(*it);
    }

    if (current.size())
    {
        ret.push_back(current);
    }

    return ret;
}
