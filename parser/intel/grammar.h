/**
 * Reaver Project Assembler License
 *
 * Copyright © 2014 Michał "Griwes" Dominiak
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
 **/

#pragma once

#include <functional>
#include <memory>

#include <boost/spirit/include/qi.hpp>

#include "../ast.h"

namespace qi = boost::spirit::qi;

namespace reaver
{
    namespace assembler
    {
        template<typename Iterator, typename Skipper>
        struct intel_grammar : qi::grammar<Iterator, void(), Skipper>
        {
            template<typename Tokens>
            intel_grammar(const Tokens & tok, ast & ast, std::function<std::shared_ptr<utils::include_chain> ()> /*chain*/,
                std::size_t & /*line*/) : intel_grammar::base_type(line)
            {
                identifier %= qi::eps >> tok.identifier;

                constant = tok.identifier[([&](const std::string & attr, const auto &, bool & parsed)
                {
                    assembler::constant ret;
                    if (ast.has_constant(attr))
                    {
                        ret.name = attr;
                        ret.value = ast.get_constant(attr);
                    }

                    parsed = false;
                    return ret;
                })];

                integer_literal = tok.binary_literal[([&](const std::string & attr, const auto &, bool & parsed)
                    {
                        boost::multiprecision::cpp_int value = 0;

                        for (const auto & elem : attr)
                        {
                            value *= 2;
                            value += elem - '0';
                        }

                        assembler::integer_literal ret;
                        ret.literal = attr;
                        ret.value = value;
                        parsed = true;

                        return ret;
                    })]
                    | tok.decimal_literal[([&](const std::string & attr, const auto &, bool & parsed)
                    {
                        boost::multiprecision::cpp_int value = 0;

                        for (const auto & elem : attr)
                        {
                            value *= 10;
                            value += elem - '0';
                        }

                        assembler::integer_literal ret;
                        ret.literal = attr;
                        ret.value = value;
                        parsed = true;

                        return ret;
                    })]
                    | tok.hexadecimal_literal[([&](const std::string & attr, const auto &, bool & parsed)
                    {
                        boost::multiprecision::cpp_int value = 0;

                        for (const auto & elem : attr)
                        {
                            value *= 16;

                            int digit;

                            if (elem >= '0' && elem <= '9')
                            {
                                digit = elem - '0';
                            }

                            else if (elem >= 'a' && elem <= 'f')
                            {
                                digit = elem - 'a' + 10;
                            }

                            else
                            {
                                digit = elem - 'A' + 10;
                            }

                            value += digit;
                        }

                        assembler::integer_literal ret;
                        ret.literal = attr;
                        ret.value = value;
                        parsed = true;

                        return ret;
                    })];

                integer_expression %= addsub | muldiv | shift | bit_and | bit_xor | bit_or;

                term %= integer | (qi::lit('(') >> integer_expression >> qi::lit(')'));

                integer %= integer_literal | constant | integer_expression;
            }

            qi::rule<Iterator, assembler::identifier(), Skipper> identifier;
            qi::rule<Iterator, assembler::constant(), Skipper> constant;
            qi::rule<Iterator, assembler::integer_literal(), Skipper> integer_literal;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> integer_expression;
            qi::rule<Iterator, assembler::integer(), Skipper> integer;
            qi::rule<Iterator, assembler::floating_point(), Skipper> floating_point;
            qi::rule<Iterator, assembler::prefix(), Skipper> prefix;
            qi::rule<Iterator, assembler::cpu_register(), Skipper> cpu_register;
            qi::rule<Iterator, assembler::operand(), Skipper> operand;
            qi::rule<Iterator, assembler::address(), Skipper> address;
            qi::rule<Iterator, assembler::instruction(), Skipper> instruction;

            qi::rule<Iterator, assembler::integer(), Skipper> term;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> addsub;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> muldiv;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> shift;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> bit_and;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> bit_xor;
            qi::rule<Iterator, assembler::integer_expression(), Skipper> bit_or;

//            qi::rule<Iterator, org_directive(), Skipper> org;
  //          qi::rule<Iterator, bits_directive(), Skipper> bits;
    //        qi::rule<Iterator, section_directive(), Skipper> section;
      //      qi::rule<Iterator, global_directive(), Skipper> global;
        //    qi::rule<Iterator, extern_directive(), Skipper> extern_;
            qi::rule<Iterator, void(), Skipper> include;

            qi::rule<Iterator, void(), Skipper> line;
        };
    }
}

