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

#include <boost/spirit/include/lex.hpp>

namespace lex = boost::spirit::lex;

namespace reaver
{
    namespace assembler
    {
        template<typename Lexer>
        struct intel_tokens : lex::lexer<Lexer>
        {
            intel_tokens()
            {
                identifier = "[a-zA-Z@_.][a-zA-Z0-9@_]*";

                binary_literal = "0b[01]+";
                decimal_literal = "[0-9]+";
                hexadecimal_literal = "0x[0-9a-fA-F]+";

                string_literal = R"~("([^"\\]*(\\.[^\"\\]*)*)")~";
                character_literal = R"('\\?.')";

                comma = R"(\.)";
                plus = R"(\+)";
                minus = R"(\-)";
                slash = R"(\/)";
                star = R"(\*)";
                percent = R"(\%)";
                dollar = R"(\$)";
                ampersand = "&";
                pipe = R"(\|)";
                dash = R"(\^)";
                tilde = R"(~)";
                question_mark = R"(\?)";
                exclamation_mark = R"(\!)";
                open_paren = R"(\()";
                close_paren = R"(\))";
                open_square = R"([)";
                close_square = R"(])";
                colon = R"(\:)";
                left_shift = R"(\<\<)";
                right_shift = R"(\>\>)";

                skip = "[ \t\r\n\v\f]+|;.*";

                this->self.add(identifier)(binary_literal)(decimal_literal)(hexadecimal_literal)(string_literal)
                    (character_literal)(comma)(plus)(minus)(slash)(star)(percent)(dollar)(ampersand)(pipe)(dash)(tilde)
                    (question_mark)(exclamation_mark)(open_paren)(close_paren)(open_square)(close_square)(colon)(left_shift)
                    (right_shift);
                this->self("skip") = skip;
            }

            lex::token_def<std::string> identifier;
            lex::token_def<std::string> binary_literal;
            lex::token_def<std::string> decimal_literal;
            lex::token_def<std::string> hexadecimal_literal;
            lex::token_def<std::string> string_literal;
            lex::token_def<std::string> character_literal;

            lex::token_def<std::string> comma;
            lex::token_def<std::string> plus;
            lex::token_def<std::string> minus;
            lex::token_def<std::string> slash;
            lex::token_def<std::string> star;
            lex::token_def<std::string> percent;
            lex::token_def<std::string> dollar;
            lex::token_def<std::string> ampersand;
            lex::token_def<std::string> pipe;
            lex::token_def<std::string> dash;
            lex::token_def<std::string> tilde;
            lex::token_def<std::string> question_mark;
            lex::token_def<std::string> exclamation_mark;
            lex::token_def<std::string> open_paren;
            lex::token_def<std::string> close_paren;
            lex::token_def<std::string> open_square;
            lex::token_def<std::string> close_square;
            lex::token_def<std::string> colon;
            lex::token_def<std::string> left_shift;
            lex::token_def<std::string> right_shift;

            lex::token_def<lex::omit> skip;
        };
    }
}
