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

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace reaver
{
    namespace assembler
    {
        struct integer
        {
            integer(boost::optional<std::string> s, boost::optional<std::string> sign, uint64_t val)
            : positive{ sign ? *sign == "+" : true }, value{ val }
            {
                if (!s)
                {
                    return;
                }

                if (*s == "byte")
                {
                    size = byte;
                }

                else if (*s == "word")
                {
                    size = word;
                }

                else if (*s == "dword")
                {
                    size = dword;
                }

                else if (*s == "qword")
                {
                    size = qword;
                }
            }

            bool positive;
            uint64_t value;
            enum
            {
                byte,
                word,
                dword,
                qword
            } size;
        };

        struct ch
        {
            ch(std::string str)
            {
                if (str[1] != '\\')
                {
                    character = str[1];
                    return;
                }

                switch (str[2])
                {
                    case 'n':
                        character = '\n';
                        return;
                    case 'r':
                        character = '\r';
                        return;
                    case 't':
                        character = '\t';
                        return;
                    case '\\':
                        character = '\\';
                        return;
                    case '\'':
                        character = '\'';
                        return;
                    case 'b':
                        character = '\b';
                        return;
                    default:
                        throw "invalid escape sequence";
                }
            }

            char character;
        };

        struct str
        {
            str(std::string str)
            {
                for (uint64_t i = 1; i < str.size() - 1; ++i)
                {
                    if (str[i] != '\\')
                    {
                        string.push_back(str[i]);
                        return;
                    }

                    switch (str[i + 1])
                    {
                        case 'n':
                            string.push_back('\n');
                            return;
                        case 'r':
                            string.push_back('\r');
                            return;
                        case 't':
                            string.push_back('\t');
                            return;
                        case '\\':
                            string.push_back('\\');
                            return;
                        case '\'':
                            string.push_back('\'');
                            return;
                        case 'b':
                            string.push_back('\b');
                            return;
                        default:
                            throw "invalid escape sequence";
                    }

                    if (i + 1 == str.size())
                    {
                        throw "invalid escape sequence";
                    }
                }
            }

            std::string string;
        };

        struct data
        {
            std::string size;
            std::vector<boost::variant<integer, ch, str>> operands;
        };
    }
}
