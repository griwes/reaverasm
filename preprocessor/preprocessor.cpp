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

#include <reaver/logger.h>
#include <reaver/style.h>

#include <utils.h>
#include <assembler.h>
#include <preprocessor/preprocessor.h>
#include <preprocessor/macro.h>

using namespace reaver::logger;
using reaver::style::style;
using namespace reaver::style;

reaver::assembler::preprocessor::preprocessor(reaver::assembler::frontend & front) : _defines(front.defines()), _frontend(front)
{
}

std::vector<reaver::assembler::line> reaver::assembler::preprocessor::preprocess(const std::string & _buffer)
{
    if (_frontend.default_includes())
    {
        for (auto & file : _frontend.get_default_includes())
        {
            std::stringstream input{_frontend.read_file(file, { std::make_pair(std::string("<command line>"), 0) }).first};

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
        if (include_chain.size() > 10)
        {
            std::exit(123);
        }

        if (buffer.back() == '\\')
        {
            print_include_chain(include_chain);
            logger::log(error) << "invalid `\\` at the end of line.";

            std::exit(-1);
        }

        if (buffer.find("%") == std::string::npos)
        {
            _lines.emplace_back(_apply_defines(_tokenize(buffer), include_chain), include_chain);
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
                        logger::log(error) << "junk after %include directive.";

                        std::exit(-1);
                    }
                }

                std::string filename = buffer.substr(opening + 1, closing - opening - 1);

                std::pair<std::string, std::string> include = _frontend.read_file(filename, include_chain);

                if (std::find_if(include_chain.begin(), include_chain.end(),
                    [include](std::pair<std::string, uint64_t> elem)
                    {
                        return elem.first == include.second;
                    }) != include_chain.end())
                {
                    print_include_chain(include_chain);
                    logger::log(error) << "file " << style::style(colors::bgray, colors::def, styles::bold) << include.second
                        << style::style() << " included recursively.";

                    std::exit(-1);
                }

                auto new_include_chain = include_chain;
                new_include_chain.emplace_back(std::make_pair(include.second, 0));

                std::stringstream sin{include.first};

                _include_stream(sin, new_include_chain);
            }

            else if (buffer.find("%define") == 0 || buffer.find("%xdefine") == 0)
            {
                std::vector<std::string> tokenized = _tokenize(remove_leading_whitespace(buffer.substr(7)));

                std::string name = tokenized[0];
                std::string definition;

                if (!_valid_macro_name(name))
                {
                    print_include_chain(include_chain);
                    logger::log(error) << "invalid macro name `" << name << "`.";

                    std::exit(-1);
                }

                if (_defines.find(name) != _defines.end())
                {
                    print_include_chain(include_chain);
                    logger::log(error) << "macro name `" << style::style(colors::bgray, colors::def, styles::bold) << name
                        << style::style() << "` redefined.";
                    print_include_chain(_defines[name].source());
                    logger::log() << "Note: first defined here.";

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
                            logger::log(error) << "macro parameter list not closed.";

                            std::exit(-1);
                        }

                        if (*it == "," || remove_leading_whitespace(*it) == std::string())
                        {
                        }

                        else if (std::find(params.begin(), params.end(), *it) != params.end())
                        {
                            print_include_chain(include_chain);
                            logger::log(error) << "parameter name reused in macro parameter list.";

                            std::exit(-1);
                        }

                        else if (_valid_macro_name(*it))
                        {
                            params.push_back(remove_leading_whitespace(*it));
                        }

                        else
                        {
                            print_include_chain(include_chain);
                            logger::log(error) << "invalid element in macro parameter list: `" << *it << "`.";

                            std::exit(-1);
                        }
                    }

                    ++it;

                    for (; it != tokenized.end(); ++it)
                    {
                        definition += *it;
                    }

                    if (buffer.find("%define") == 0)
                    {
                        _defines.emplace(name, define(name, params, remove_leading_whitespace(definition), include_chain));
                    }

                    else
                    {
                        _defines.emplace(name, define(name, params, _apply_defines(_tokenize(remove_leading_whitespace(definition)),
                            include_chain), include_chain));
                    }
                }

                else
                {
                    for (auto it = tokenized.begin() + 1; it != tokenized.end(); ++it)
                    {
                        definition += *it;
                    }

                    if (buffer.find("%define") == 0)
                    {
                        _defines.emplace(name, define(name, remove_leading_whitespace(definition), include_chain));
                    }

                    else
                    {
                        _defines.emplace(name, define(name, _apply_defines(_tokenize(remove_leading_whitespace(definition)),
                            include_chain), include_chain));
                    }
                }
            }

            else if (buffer.find("%assign") == 0)
            {

            }

            else if (buffer.find("%undef") == 0)
            {

            }

            else if (buffer.find("%assign") == 0)
            {

            }

            else if (buffer.find("%strlen") == 0)
            {

            }

            else if (buffer.find("%substr") == 0)
            {

            }

            else if (buffer.find("%macro") == 0)
            {

            }

            else if (buffer.find("%if") == 0)
            {

            }

            else if (buffer.find("%error") == 0)
            {
                print_include_chain(include_chain);
                logger::log() << style::style(colors::bred, colors::def, styles::bold) << "User defined error: '"
                    << style::style() << buffer.substr(7) << "'.";

                std::exit(-1);
            }

            else if (buffer.find("%rep") == 0)
            {

            }

            else if (buffer.find("%push") == 0)
            {

            }

            else if (buffer.find("%pop") == 0)
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
            if (current.size() == 0 && ret.size() != 0)
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

        if (*it == ',' || *it == '(' || *it == ')' || *it == '[' || *it == ']')
        {
            ret.push_back(current);
            ret.push_back({*it});
            current.clear();

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

std::string reaver::assembler::preprocessor::_apply_defines(const std::vector<std::string> & tokens,
    const std::vector<std::pair<std::string, uint64_t>> & include_chain)
{
    std::string ret;

    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (_defines.find(*it) != _defines.end())
        {
            if (std::find(_define_stack.begin(), _define_stack.end(), *it) != _define_stack.end())
            {
                print_include_chain(include_chain);
                logger::log(error) << "macro `" << style::style(colors::white, colors::def, styles::bold) << *it << style::style()
                    << "` used recursively.";

                std::exit(-1);
            }

            _define_stack.push_back(*it);

            if (_defines[*it].parameters().size())
            {
                auto definition = _tokenize(_defines[*it].definition());

                if (*(it + 1) != "(")
                {
                    ret.append(*it);

                    continue;
                }

                auto close = std::find(it + 2, tokens.end(), ")");

                if (close == tokens.end())
                {
                    print_include_chain(include_chain);
                    logger::log(error) << "closing brace not found.";

                    std::exit(-1);
                }

                ++it;

                for (auto param : _defines[_define_stack.back()].parameters())
                {
                    if (*it == "(")
                    {
                        ++it;
                    }

                    while (*it == " ")
                    {
                        ++it;
                    }

                    if (*it == ",")
                    {
                        print_include_chain(include_chain);
                        logger::log(error) << "macro argument cannot be empty.";

                        std::exit(-1);
                    }

                    std::string arg = *it;

                    if (param != *(_defines[_define_stack.back()].parameters().end() - 1))
                    {
                        while (*++it != "," && *it != ")")
                        {
                            arg.append(*it);
                        }

                        if (*it == ")")
                        {
                            print_include_chain(include_chain);
                            logger::log(error) << "not enough arguments for macro `" << style::style(colors::white,
                                colors::def, styles::bold) << _define_stack.back() << style::style() << "`.";

                            std::exit(-1);
                        }

                        ++it;
                    }

                    else
                    {
                        while (*++it != ")" && *it != ",")
                        {
                            arg.append(*it);
                        }

                        if (*it == ",")
                        {
                            print_include_chain(include_chain);
                            logger::log(error) << "too many arguments for macro `" << style::style(colors::white,
                                colors::def, styles::bold) << _define_stack.back() << style::style() << "`.";

                            std::exit(-1);
                        }

                        ++it;
                    }

                    arg = _apply_defines(_tokenize(arg), include_chain);

                    std::replace(definition.begin(), definition.end(), param, arg);

                }

                for (auto & x : definition)
                {
                    ret.append(x);
                }
            }

            else
            {
                ret.append(_apply_defines(_tokenize(_defines[*it].definition()), include_chain));
            }

            _define_stack.pop_back();
        }

        else
        {
            ret.append(*it);
        }
    }

    return ret;
}
