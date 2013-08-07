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

#include <reaver/lexer.h>

namespace reaver
{
    namespace assembler
    {
        enum nasm_preprocessing_tokens
        {
            pp_directive,
            pp_identifier,
            pp_number,
            pp_character,
            pp_string,
            pp_symbol,
            pp_whitespace,
            pp_comment
        };

        struct nasm_preprocessor_lexer
        {
            nasm_preprocessor_lexer() : directive{ reaver::assembler::pp_directive, "%[a-z]+" },
                identifier{ reaver::assembler::pp_identifier, "[a-zA-Z_.][a-zA-Z0-9_.]*" },
                number{ reaver::assembler::pp_number, "(0x[0-9a-fA-F]+)|(0b[01]+)|([0-9a-fA-F]+[hH])|([0-9]*\\.[0-9]*)|([0-9]+)" },
                character{ reaver::assembler::pp_character, "'\\\\?.'" },
                string{ reaver::assembler::pp_string, "\"([^\"\\\\]*(\\.[^\"\\\\]*)*)\"" },
                symbol{ reaver::assembler::pp_symbol, "[[:punct:]]" },
                whitespace{ reaver::assembler::pp_whitespace, "[ \t\r\n\v\f]+" },
                comment{ reaver::assembler::pp_comment, ";.*", [](std::string) { return ""; } }
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
            lexer::token_definition<std::string> comment;

            lexer::basic_tokens_description<char> desc;
        };
    }
}