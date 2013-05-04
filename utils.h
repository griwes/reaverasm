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

#include <iostream>
#include <vector>

#include <reaver/logger.h>
#include <reaver/style.h>

#include <include_chain.h>

using namespace reaver::logger;
using namespace reaver::style;

inline void print_include_chain(const reaver::assembler::include_chain & include_chain, reaver::logger::logger & l = dlog)
{
    if (include_chain.empty())
    {
        return;
    }

    l() << "In file " << style(colors::bgray, colors::def, styles::bold) << include_chain.back().name
        << style() << " in line " << include_chain.back().line << ":";

    for (auto it = include_chain.crbegin() + 1; it != include_chain.crend(); ++it)
    {
        if (!it->macro)
        {
            l() << "Included from " << it->name << " in line " << it->line << ":";
        }

        else
        {
            l() << "In expanded preprocessor directive `" << it->name << "` in line " << it->line << ":";
        }
    }
}

inline std::string remove_leading_whitespace(const std::string & str)
{
    auto it = begin(str);

    while (it != end(str) && std::isspace(*it))
    {
        ++it;
    }

    return { it, end(str) };
}

namespace reaver
{
    namespace assembler
    {
        class exception : public std::exception, public reaver::logger::logger_friend
        {
        public:
            exception(level l = always, include_chain inc = include_chain{}) : _inc{ inc }
            {
                using reaver::style::style;
                using reaver::style::colors;
                using reaver::style::styles;

                switch (l)
                {
                    case trace:
                        _strings = { std::make_pair(style(colors::bgray), "Trace: "), std::make_pair(style(), "") };
                        break;
                    case debug:
                        _strings = { std::make_pair(style(colors::gray), "Debug: "), std::make_pair(style(), "") };
                        break;
                    case info:
                        _strings = { std::make_pair(style(), "Info: ") };
                        break;
                    case warning:
                        _strings = { std::make_pair(style(colors::bbrown, colors::def, styles::bold), "Warning: "),
                            std::make_pair(style(), "") };
                        break;
                    case error:
                        _strings = { std::make_pair(style(colors::bred, colors::def, styles::bold), "Error: "),
                            std::make_pair(style(), "") };
                        break;
                    case crash:
                        _strings = { std::make_pair(style(colors::bred, colors::def, styles::bold), "Internal error: "),
                            std::make_pair(style(), "") };
                        break;
                    case always:
                        ;
                }
            }

            exception(include_chain inc) : exception{ always, inc }
            {
            }

            ~exception()
            {
            }

            exception & operator<<(const char * str)
            {
                _strings.back().second.append(str);
                return *this;
            }

            exception & operator<<(const std::string & str)
            {
                _strings.back().second.append(str);
                return *this;
            }

            template<typename T>
            exception & operator<<(const T & rhs)
            {
                _strings.back().second.append(std::to_string(rhs));
                return *this;
            }

            exception & operator<<(const reaver::style::style & style)
            {
                _strings.push_back(std::make_pair(style, ""));
                return *this;
            }

            virtual const char * what() const noexcept
            {
                print(dlog);

                return "reaver::assembler::exception\ncall e.print(<logger object>) to get more detailed output";
            }

            virtual void print(reaver::logger::logger & l) const noexcept
            {
                print_include_chain(_inc, l);

                auto strings = _strings;

                for (auto & stream : _streams(l))
                {
                    _async(l, [=]() mutable
                    {
                        for (auto x : strings)
                        {
                            stream << x.first;
                            stream << x.second;
                        }

                        stream << "\n";
                    });
                }
            }

            friend reaver::logger::logger & operator<<(reaver::logger::logger &, const exception &);

            level level() const
            {
                return _level;
            }

        private:
            ::level _level;

            include_chain _inc;
            std::vector<std::pair<reaver::style::style, std::string>> _strings;
        };

        class exception_note : public exception
        {
        public:
            exception_note(const exception & exc, const exception & note) : exception{ exc }, _note{ note }
            {
            }

            virtual void print(reaver::logger::logger & l) const noexcept
            {
                exception::print(l);
                _note.print(l);
            }

            ::level level() const
            {
                return exception::level();
            }

        private:
            exception _note;
        };

        inline reaver::logger::logger & operator<<(reaver::logger::logger & l, const exception & e)
        {
            e.print(l);
            return l;
        }
    }
}
