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

#include <reaver/exception.h>
#include <reaver/lexer.h>

#include <preprocessor/nasm/nasm.h>

using reaver::style::style;
using reaver::style::colors;
using reaver::style::styles;

namespace
{
    enum if_state
    {
        if_true,
        if_false,
        elif_true,
        elif_false,
        else_true,
        else_false
    };
}

namespace reaver
{
    namespace assembler
    {
        struct nasm_preprocessor_state
        {
            std::vector<line> lines;
            std::vector<std::string> define_stack;
            std::map<std::string, define> defines;
            std::map<std::string, std::shared_ptr<macro>> macros;
            std::vector<if_state> if_state_stack;
        };
    }
}

std::vector<reaver::assembler::line> reaver::assembler::nasm_preprocessor::operator()() const
{
    nasm_preprocessor_state state;
    state.defines = _front.defines();

    if (_front.default_includes().size())
    {
        auto cmdline_inc = std::make_shared<utils::include_chain>("<command line>");

        for (auto & x : _front.default_includes())
        {
            _include_stream(x.stream, state, std::make_shared<utils::include_chain>(x.name, cmdline_inc));
        }
    }

    _include_stream(_front.input(), state, std::make_shared<utils::include_chain>(_front.input_name()));

    return state.lines;
}

void reaver::assembler::nasm_preprocessor::_include_stream(std::istream & input, reaver::assembler::nasm_preprocessor_state & state,
    std::shared_ptr<reaver::assembler::utils::include_chain> inc) const
{
    if (inc->size > 1024)
    {
        throw exception(error) << "maximum include (or macro) depth reached.";
    }

    std::string buffer;
    uint64_t new_lines = 1;
    uint64_t current_line = 0;

    auto chain = [&](){
        auto i = std::make_shared<utils::include_chain>(*inc);
        i->line = current_line;
        return i;
    };

    while (std::getline(input, buffer) && (current_line += new_lines))
    {
        new_lines = 1;
        std::vector<std::string> v{ buffer };

        while (buffer.size() && buffer.back() == '\\')
        {
            buffer.pop_back();

            std::string next;

            if (!std::getline(input, next))
            {
                throw exception(error) << "invalid `\\` at the end of file.";
            }

            buffer.append(next);
            v.push_back(next);
            ++new_lines;
        }

        if (buffer.empty())
        {
            continue;
        }

        auto tokens = reaver::lexer::tokenize(buffer, _lexer.desc);
        auto begin = tokens.cbegin();

        {
            auto if_match = parser::parse(_parser.if_directive, begin, tokens.cend(), _parser.skip);

            if (if_match)
            {
                if (if_match->type == "%if")
                {
                    throw exception(crash) << "%if directive is not implemented yet.";
                }

                else
                {
                    if (state.defines.count(if_match->condition))
                    {
                        state.if_state_stack.push_back(if_true);
                    }

                    else
                    {
                        state.if_state_stack.push_back(if_false);
                    }

                    continue;
                }
            }
        }

        {
            begin = tokens.cbegin();
            auto elif = parser::parse(_parser.elseif, begin, tokens.cend(), _parser.skip);

            if (elif && (state.if_state_stack.empty() || state.if_state_stack.back() == else_true || state.if_state_stack.back()
                == else_false))
            {
                throw exception(error) << "invalid %elif directive.";
            }

            throw exception(crash) << "%elif directive is not implemented yet.";
        }

        {
            begin = tokens.cbegin();
            auto else_match = parser::parse(_parser.else_directive, begin, tokens.cend(), _parser.skip);

            if (else_match)
            {
                if (state.if_state_stack.empty() || state.if_state_stack.back() == else_true || state.if_state_stack.back()
                    == else_false)
                {
                    throw exception(error) << "invalid %else directive.";
                }

                if (state.if_state_stack.back() == if_true || state.if_state_stack.back() == elif_true)
                {
                    state.if_state_stack.back() = else_false;
                }

                else
                {
                    state.if_state_stack.back() = else_true;
                }

                continue;
            }
        }

        {
            begin = tokens.cbegin();
            auto endif = parser::parse(_parser.endif, begin, tokens.cend(), _parser.skip);

            if (endif)
            {
                if (state.if_state_stack.size())
                {
                    state.if_state_stack.pop_back();
                }

                else
                {
                    throw exception(error) << "invalid %endif directive.";
                }
            }
        }

        {
            auto find = [&](if_state s){
                return std::find(state.if_state_stack.begin(), state.if_state_stack.end(), s) != state.if_state_stack.end();
            };

            if (state.if_state_stack.size() && (find(if_false) || find(elif_false) || find(else_false)))
            {
                continue;
            }
        }

        {
            begin = tokens.cbegin();
            auto define = reaver::parser::parse(_parser.define, begin, tokens.cend(), _parser.skip);

            if (define)
            {
                if (state.defines.count(define->name))
                {
                    throw exception(error) << "define name `" << style::style(colors::bgray, colors::def, styles::bold)
                        << define->name << style::style() << "` redefined.";
                }

                if (define->directive == "%define" && !define->args)
                {
                    std::string definition;

                    for (; begin != tokens.cend(); ++begin)
                    {
                        definition.append(begin->as<std::string>());
                    }

                    state.defines.emplace(define->name, assembler::define{ define->name, definition, chain() });
                }

                else if (define->directive == "%xdefine" && !define->args)
                {
                    auto i = chain();
                    state.defines.emplace(define->name, assembler::define{ define->name, _apply_defines(begin, tokens.cend(),
                        state, i), i });
                }

                else if (define->directive == "%define")
                {
                    std::string definition;

                    while (begin != tokens.cend())
                    {
                        definition.append((begin++)->as<std::string>());
                    }

                    state.defines.emplace(define->name, assembler::define{ define->name, *define->args, definition,
                        chain() });
                }

                else
                {
                    auto i = chain();
                    state.defines.emplace(define->name, assembler::define{ define->name, *define->args, _apply_defines(begin,
                        tokens.cend(), state, i), i });
                }

                continue;
            }
        }

        {
            begin = tokens.cbegin();
            auto include = parser::parse(_parser.include, begin, tokens.cend(), _parser.skip);

            if (include)
            {
                auto file = _front.open_file(include->substr(1, include->size() - 2));

                if (begin != tokens.cend())
                {
                    throw exception(error) << "junk after %include directive.";
                }

                auto i = std::make_shared<utils::include_chain>(file.name, chain(), 0, false);
                _include_stream(file.stream, state, i);

                continue;
            }
        }

        {
            begin = tokens.cbegin();
            auto undef = parser::parse(_parser.undef, begin, tokens.cend(), _parser.skip);

            if (undef)
            {
                if (begin != tokens.cend())
                {
                    throw exception(error) << "junk after %undef directive.";
                }

                if (state.defines.find(*undef) == state.defines.end())
                {
                    throw exception(error) << "unknown define: `" << style::style(colors::white, colors::def,
                        styles::bold) << *undef << style::style() << "`.";
                }

                state.defines.erase(state.defines.find(*undef));

                continue;
            }
        }

        {
            begin = tokens.cbegin();

            if (begin->type() == assembler::pp_directive && begin->as<std::string>() == "%error")
            {
                std::string error;

                while ((++begin)->type() == assembler::pp_whitespace) {}

                while (begin != tokens.cend())
                {
                    error.append((begin++)->as<std::string>());
                }

                throw exception(logger::error) << error;
            }
        }

        {
            begin = tokens.cbegin();
            while ((begin++)->type() == pp_whitespace) {}

            if (begin->type() == pp_directive)
            {
                throw exception(error) << "unknown preprocessor directive: `" << style::style(colors::bgray, colors::def,
                    styles::bold) << begin->as<std::string>() << style::style() << "`.";
            }
        }

        state.lines.emplace_back(_apply_defines(buffer, state, chain()), v, current_line, inc);
    }
}

std::string reaver::assembler::nasm_preprocessor::_apply_defines(std::string str, reaver::assembler::nasm_preprocessor_state
    & state, std::shared_ptr<reaver::assembler::utils::include_chain> inc) const
{
    auto tokens = lexer::tokenize(str, _lexer.desc);
    return _apply_defines(tokens.begin(), tokens.end(), state, inc);
}

std::string reaver::assembler::nasm_preprocessor::_apply_defines(std::vector<lexer::token>::const_iterator begin,
    std::vector<lexer::token>::const_iterator end, reaver::assembler::nasm_preprocessor_state & state, std::shared_ptr<
    reaver::assembler::utils::include_chain> inc) const
{
    std::string ret;

    while (begin != end)
    {
        if (state.defines.find(begin->as<std::string>()) != state.defines.end())
        {
            if (std::find(state.define_stack.begin(), state.define_stack.end(), begin->as<std::string>())
                != state.define_stack.end())
            {
                throw exception(error) << "define `" << style::style(colors::white, colors::def, styles::bold) <<
                    begin->as<std::string>() << style::style() << "` defined recursively.";
            }

            auto & define = state.defines.at(begin->as<std::string>());
            state.define_stack.push_back(begin->as<std::string>());

            if (define.parameters().size())
            {
                auto t = begin;
                auto match = reaver::parser::parse(_parser.define_call, t, end, _parser.skip);

                if (match)
                {
                    begin = t - 1;

                    if (match->args.size() != define.parameters().size())
                    {
                        throw exception(error) << "wrong number of parameters for define `" << style::style(colors::white,
                            colors::def, styles::bold) << define.name() << style::style() << "`; expected " << define.parameters()
                            .size() << ", got " << match->args.size() << ".";
                    }

                    std::map<std::string, std::string> params;
                    for (uint64_t i = 0; i < define.parameters().size(); ++i)
                    {
                        params[define.parameters()[i]] = match->args[i];
                    }

                    for (auto & x : lexer::tokenize(define.definition(), _lexer.desc))
                    {
                        if (params.find(x.as<std::string>()) != params.end())
                        {
                            ret.append(_apply_defines(params[x.as<std::string>()], state, inc));
                        }

                        else
                        {
                            ret.append(_apply_defines(x.as<std::string>(), state, inc));
                        }
                    }
                }

                else
                {
                    throw exception(error) << "no parameters supplied for macro `" << style::style(colors::white,
                        colors::def, styles::bold) << define.name() << style::style() << "` taking " << define.parameters()
                        .size() << " arguments.";
                }
            }

            else
            {
                ret.append(_apply_defines(define.definition(), state, inc));
            }
        }

        else
        {
            ret.append(begin->as<std::string>());
        }

        ++begin;
    }

    return ret;
}
