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
        class cpu_register;
        struct integer;
        struct size_overriden_identifier;
        class effective_address;

        struct operand_base
        {
            virtual ~operand_base()
            {
            }

            virtual const cpu_register & get_register() const
            {
                throw "get_register() on non-register operand, consider this internal error";
            }

            virtual std::string name() const
            {
                throw "name() on unnamed operand, consider this internal error";
            }

            virtual uint64_t size() const = 0;
        };

        struct integer : public operand_base
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
                    size_enum = byte;
                }

                else if (*s == "word")
                {
                    size_enum = word;
                }

                else if (*s == "dword")
                {
                    size_enum = dword;
                }

                else if (*s == "qword")
                {
                    size_enum = qword;
                }
            }

            integer(uint8_t i) : positive{ true }, value{ i }
            {
            }

            virtual uint64_t size() const
            {
                switch (size_enum)
                {
                    case byte:
                        return 8;
                    case word:
                        return 16;
                    case dword:
                        return 32;
                    case qword:
                        return 64;
                    case implicit:
                        throw "TODO: implement implicit size() of an integer";
                }
            }

            bool positive;
            uint64_t value;
            enum
            {
                byte,
                word,
                dword,
                qword,
                implicit
            } size_enum = implicit;
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

            operator integer() const
            {
                return static_cast<uint8_t>(character);
            }
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
