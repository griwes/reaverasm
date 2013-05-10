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
#include <cpu/instruction.h>
#include <cpu/cpu.h>

#include <utils.h>

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
                integer_literal{ reaver::assembler::integer_literal, "(0x[0-9a-fA-F]+)|([0-9a-fA-F]+[hH])|(0b[01]+)|([0-9]+)",
                    [](const std::string & str) -> uint64_t
                    {
                        if (str[0] == '0' && str[1] == 'b')
                        {
                            std::bitset<64> set{ str.substr(2) };
                            return set.to_ullong();
                        }

                        std::stringstream s{ str };
                        uint64_t a = 0;

                        if ((str[0] == '0' && str[1] == 'x') || str.back() == 'h' || str.back() == 'H')
                        {
                            s >> std::hex;
                        }

                        s >> a;

                        return a;
                    }
                },
                fp_literal{ reaver::assembler::fp_literal, "0.00" },
                character{ reaver::assembler::character, "'\\\\?.'" },
                string{ reaver::assembler::string, "\"([^\"\\\\]*(\\.[^\"\\\\]*)*)\"" },
                symbol{ reaver::assembler::symbol, "[[:punct:]]" },
                whitespace{ reaver::assembler::whitespace, "[ \t\r\n\v\f]+" }
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
                integer = -override_size >> -symbol({ "+", "-" }) >> reaver::parser::token(lex.integer_literal);
                fp = reaver::parser::token(lex.fp_literal);
                character = reaver::parser::token(lex.character);
                string = reaver::parser::token(lex.string);
                symbol = reaver::parser::token(lex.symbol);

                not_a_register = identifier - (cpureg | segment_register | size | identifier({ "bits", "extern",
                    "global", "org", "section" }));
                label_definition = not_a_register >> ~symbol({ ":" });

                size = identifier({ "byte", "word", "dword", "qword" });
                cpureg = identifier(byte_registers()) | identifier(word_registers()) | identifier(dword_registers())
                    | identifier(qword_registers());
                segment_register = identifier(segment_registers());

                override_segment = segment_register >> ~symbol({ ":" });
                override_size = size;

                override_symbol_size = -override_size >> not_a_register;
                address_operand = cpureg | integer | override_symbol_size;

                // TODO for addresses: allow additional math operators for assemble-time constants
                address = -override_size >> ~symbol({ "[" }) >> -override_size >> -override_segment > address_operand >>
                    *(symbol({ "+", "-", "*", "/" }) > address_operand) >> ~symbol({ "]" });

                instruction_operand = override_symbol_size | address | cpureg | integer;
                instruction_mnemonic = identifier(get_known_mnemonics());

                assembly_instruction = instruction_mnemonic >> -(instruction_operand % symbol({ "," }));

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
            reaver::parser::rule<cpu_register> cpureg;
            reaver::parser::rule<cpu_register> segment_register;

            // TODO: FP, SSE, AVX and other shenaniganish registers

            reaver::parser::rule<size_overriden_identifier> override_symbol_size;
            reaver::parser::rule<effective_address_operand> address_operand;

            reaver::parser::rule<cpu_register> override_segment;
            reaver::parser::rule<std::string> override_size;
            reaver::parser::rule<effective_address> address;

            reaver::parser::rule<operand> instruction_operand;
            reaver::parser::rule<mnemonic> instruction_mnemonic;
            reaver::parser::rule<instruction> assembly_instruction;

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