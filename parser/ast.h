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

#include <string>

#include <boost/fusion/adapted.hpp>
#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <reaver/logger.h>

#include "../utils/include_chain.h"

namespace reaver
{
    namespace assembler
    {
        struct location
        {
            std::shared_ptr<utils::include_chain> include_chain;
            std::size_t column;
        };

        struct identifier : location
        {
            std::string name;
        };

        struct constant : location
        {
            std::string name;
            boost::multiprecision::cpp_int value;
        };

        struct integer_literal : location
        {
            std::string literal;
            boost::multiprecision::cpp_int value;
        };

        struct integer_expression;
        using integer = boost::variant<integer_literal, integer_expression, constant>;

        struct integer_expression : location
        {
            std::string literal;
            boost::recursive_wrapper<integer> first_operand;
            std::string op;
            boost::recursive_wrapper<integer> second_operand;
        };

        struct floating_point : location
        {
            std::string literal;
            boost::multiprecision::cpp_rational value;
        };

        struct prefix : location
        {
            std::string prefix;
        };

        struct cpu_register
        {
            std::string name;
            // TODO
        };

        struct address;
        using operand = boost::variant<integer, floating_point, address, cpu_register, identifier, constant>;

        struct address : location
        {
            boost::optional<boost::variant<integer, cpu_register, constant>> segment;
            boost::optional<cpu_register> base;
            boost::optional<boost::variant<integer, constant>> scale;
            boost::optional<boost::variant<cpu_register, integer, constant>> index;
        };

        struct label : location
        {
            identifier label;
        };

        struct instruction : location
        {
            boost::optional<prefix> prefix;
            std::vector<operand> operands;
        };

        class ast
        {
        public:
            ast()
            {
            }

            void append(const ast &);
            void append(ast &&);

            bool has_constant(const std::string &) { throw std::runtime_error("TODO"); }
            boost::multiprecision::cpp_int get_constant(const std::string &) { throw std::runtime_error("TODO"); }
        };
    }
}

BOOST_FUSION_ADAPT_STRUCT(reaver::assembler::identifier,
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(reaver::assembler::constant,
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(reaver::assembler::integer_expression,
    (reaver::assembler::integer, first_operand)
    (std::string, op)
    (reaver::assembler::integer, second_operand)
)

