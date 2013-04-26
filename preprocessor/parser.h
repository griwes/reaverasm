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

#include <reaver/parser/parser.h>

namespace reaver
{
    namespace assembler
    {
        struct define_match
        {
            std::string directive;
            std::string name;
            boost::optional<std::vector<std::string>> args;
        };

        struct assign_match
        {
            std::string name;
            std::string value;
        };

        struct if_match
        {
            std::string type;
            std::string condition;
        };

        struct rep_match
        {
            std::string dummy;
        };

        enum tokens
        {
            directive,
            identifier,
            number,
            character,
            string,
            symbol,
            whitespace
        };

        struct preprocessor_lexer
        {
            preprocessor_lexer() : directive{ assembler::directive, "%[a-z]+" }, identifier{ assembler::identifier,
                "\\.?[a-zA-Z_][a-zA-Z0-9_]*" }, number{ assembler::number, "([0-9]+)|(0x[0-9a-fA-F]+)|(0b[01]+)|"
                "([0-9a-fA-F]+h)|([0-9]*\\.[0-9]*)" }, character{ assembler::character, "'.'" }, string{ assembler::string,
                "\"([^\"\\\\]*(\\.[^\"\\\\]*)*)\"" }, symbol{ assembler::symbol, "[\\]\\[!&*+/<=>\\^|~-(),]" },
                whitespace{ assembler::whitespace, "[ \t\r\n\v\f]+" }
            {
                desc.add(directive)(identifier)(number)(character)(string)(symbol)(whitespace);
            }

            lexer::token_definition<std::string> directive;
            lexer::token_definition<std::string> identifier;
            lexer::token_definition<std::string> number;
            lexer::token_definition<std::string> character;
            lexer::token_definition<std::string> string;
            lexer::token_definition<std::string> symbol;
            lexer::token_definition<std::string> whitespace;

            lexer::tokens_description desc;
        };

        struct preprocessor_parser
        {
            preprocessor_parser(const preprocessor_lexer & lex)
            {
                define = parser::token(lex.directive)({ "%define", "%xdefine" }) >> parser::token(lex.identifier) >>
                    -(~parser::token(lex.symbol)({ "(" }) >> parser::token(lex.identifier) % parser::token(lex.symbol)({ "," })
                    >> ~parser::token(lex.symbol)({ ")" }));
                include = ~parser::token(lex.directive)({ "%include" }) >> parser::token(lex.string);
                undef = ~parser::token(lex.directive)({ "%undef" }) >> parser::token(lex.identifier);
                error = ~parser::token(lex.directive)({ "%error" }) >> parser::token(lex.string);
                assign = ~parser::token(lex.directive)({ "%assign" }) >> parser::token(lex.identifier) >> parser::token(lex.number);
                macro = ~parser::token(lex.directive)({ "%macro" }) >> parser::token(lex.identifier);
                endmacro = parser::token(lex.directive)({ "%endmacro" });           // parser TODO: `rule<>`
                if_directive = parser::token(lex.directive)({ "%if", "%ifdef" });   // TODO
                elseif = parser::token(lex.directive)({ "%elseif", "%elif" });      // TODO
                else_directive = parser::token(lex.directive)({ "%else" });         // TODO
                rep = parser::token(lex.directive)({ "%rep" });                     // TODO
            }

            parser::rule<assembler::define_match> define;
            parser::rule<std::string> include;
            parser::rule<std::string> undef;
            parser::rule<std::string> error;
            parser::rule<assembler::assign_match> assign;
            parser::rule<std::string> macro;
            parser::rule<std::string> endmacro;                 // rule<>
            parser::rule<assembler::if_match> if_directive;
            parser::rule<std::string> elseif;
            parser::rule<std::string> else_directive;           // rule<>
            parser::rule<assembler::rep_match> rep;

            parser::rule<std::string> skip;
        };
    }
}
