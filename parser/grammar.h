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

#include <reaver/parser/parser.h>

#include <cpu/address.h>
#include <cpu/overrides.h>
#include <cpu/register.h>

namespace reaver
{
    namespace assembler
    {
        enum tokens
        {
            identifier,
            integer_literal,
            fp_literal,
            character,
            string,
            symbol,
            whitespace
        };

        struct lexer
        {
            lexer() : identifier{ reaver::assembler::identifier, "\\.?[a-zA-Z_][0-9a-zA-Z_]*" },
                integer_literal{ reaver::assembler::integer_literal, "(0x[0-9a-fA-F]+)|([0-9a-fA-F]+[hH])|(0b[01]+)|[0-9]+)" },
                fp_literal{ reaver::assembler::fp_literal, "" },
                character{ reaver::assembler::character, "'\\\\?.'" },
                string{ reaver::assembler::string, "\"([^\"\\\\]*(\\.[^\"\\\\]*)*)\"" },
                symbol{ reaver::assembler::symbol, "[[:punct:]]" },
                whitespace{ reaver::assembler::symbol, "[ \t\r\n\v\f]+" }
            {
                desc.add(identifier)(integer_literal)(fp_literal)(character)(string)(symbol)(whitespace);
            }

            reaver::lexer::token_definition<std::string> identifier;
            reaver::lexer::token_definition<uint64_t> integer_literal;
            reaver::lexer::token_definition<long double> fp_literal; // TODO
            reaver::lexer::token_definition<std::string> character;
            reaver::lexer::token_definition<std::string> string;
            reaver::lexer::token_definition<std::string> symbol;
            reaver::lexer::token_definition<std::string> whitespace;

            reaver::lexer::tokens_description desc;
        };

        struct integer
        {
            integer(boost::optional<std::string> sign, uint64_t val) : positive{ sign ? *sign == "+" : true }, value{ val }
            {
            }

            bool positive;
            uint64_t value;
        };

        struct ch
        {
            ch(std::string str)
            {
                if (str[0] != '\\')
                {
                    character = str[0];
                    return;
                }

                switch (str[1])
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
                for (uint64_t i = 0; i < str.size(); ++i)
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

        struct parser
        {
            parser(const reaver::assembler::lexer & lex)
            {
                identifier = reaver::parser::token(lex.identifier);
                integer = -symbol({ "+", "-" }) >> reaver::parser::token(lex.integer_literal);
                fp = reaver::parser::token(lex.fp_literal);
                character = reaver::parser::token(lex.character);
                string = reaver::parser::token(lex.string);
                symbol = reaver::parser::token(lex.symbol);

                not_a_register = identifier - (register64 | segment_register | size | identifier({ "bits", "extern",
                    "global", "org", "section" }));
                label_definition = not_a_register >> ~symbol({ ":" });

                size = identifier({ "byte", "word", "dword", "qword" });
                register16 = identifier({ "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "ax", "bx", "cx", "dx",
                    "bp", "sp", "di", "si" });
                register32 = identifier({ "eax", "ebx", "ecx", "edx", "ebp", "esp", "esi", "edi" }) | register16;
                register64 = identifier({ "rax", "rbx", "rcx", "rdx", "rbp", "rsp", "rsi", "rdi", "r8", "r9", "r10",
                    "r11", "r12", "r13", "r14", "r15", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
                    "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w", "r8d", "r9d", "r10d", "r11d", "r12d",
                    "r13d", "r14d", "r15d" }) | register32;
                segment_register = identifier({ "cs", "ds", "es", "fs", "gs", "ss" });

                override_segment = segment_register >> ~symbol({ ":" });
                override_size = size;

                // TODO for addresses: allow additional math operators for assemble-time constants
                address = ~symbol({ "[" }) >> -override_size >> -override_segment > (register32 | integer
                    | (-override_size >> not_a_register)) >> *(symbol({ "+", "-", "*", "/" }) > (register32 | integer
                    | (-override_size >> not_a_register))) >> ~symbol({ "]" });
                address64 = ~symbol({ "[" }) >> -override_size >> -override_segment > (register64 | integer
                    | (-override_size >> not_a_register)) >> *(symbol({ "+", "-", "*", "/" }) > (register64 | integer
                    | (-override_size >> not_a_register))) >> ~symbol({ "]" });

                bits_directive = ~identifier({ "bits" }) > reaver::parser::token(lex.integer_literal);
                extern_directive = ~identifier({ "extern" }) > not_a_register;
                global_directive = ~identifier({ "global" }) > not_a_register;
                org_directive = ~identifier({ "org" }) > reaver::parser::token(lex.integer_literal);
                section_directive = ~identifier({ "section" }) > reaver::parser::token(lex.string);

                skip = reaver::parser::token(lex.whitespace);
            }

            reaver::parser::rule<std::string> identifier;
            reaver::parser::rule<reaver::assembler::integer> integer;
            reaver::parser::rule<long double> fp;
            reaver::parser::rule<ch> character;
            reaver::parser::rule<str> string;
            reaver::parser::rule<std::string> symbol;

            reaver::parser::rule<std::string> not_a_register;
            reaver::parser::rule<std::string> label_definition;

            reaver::parser::rule<std::string> size;
            reaver::parser::rule<cpu_register> register16;
            reaver::parser::rule<cpu_register> register32;
            reaver::parser::rule<cpu_register> register64;
            reaver::parser::rule<cpu_register> segment_register;

            // TODO: FP, SSE, AVX and other shenaniganish registers

            reaver::parser::rule<segment_override> override_segment;
            reaver::parser::rule<size_override> override_size;
            reaver::parser::rule<effective_address> address; // 16 and 32
            reaver::parser::rule<effective_address> address64;

            // the following should be nullary, and soon will be nullary, but it will require some additional features
            // for `reaver::parser::rule`
            reaver::parser::rule<uint64_t> bits_directive;
            reaver::parser::rule<std::string> extern_directive;
            reaver::parser::rule<std::string> global_directive;
            reaver::parser::rule<uint64_t> org_directive;
            reaver::parser::rule<std::string> section_directive;

            reaver::parser::rule<std::string> skip;
        };
    }
}
