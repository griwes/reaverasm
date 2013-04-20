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

#include <parser/parser.h>
#include <utils.h>

namespace lex = reaver::lexer;
namespace par = reaver::parser;

const std::vector<std::string> reaver::assembler::directive::values{"org", "extern", "global", "section", "bits"};

reaver::assembler::parser::parser(reaver::assembler::frontend & front) : _frontend(front)
{
}

// parser tokenizes into:
//  1) directive, like "org", "extern", "global", "section" and alike
//  2) label
//  3) prefix, like "rep**" or "lock"
//  4) address of symbol, referring to a label or extern (symbol check happens in generator)
//  5) string literal
//  6) character literal
//  7) binary literal
//  8) dec literal
//  9) hex literal
//  10) register name
//  11) size override
//  12) address (either single literal, or things like [rax + 4 * rbx])
//  13) operator, like +, -, *, /, <<, >>, &, ^, |
//  14) segment override
//  15) instruction mnemonic
//  16) comma (is not returned in that vector of tokens)
//  17) operand

reaver::assembler::ast reaver::assembler::parser::parse(const std::vector<reaver::assembler::line> & lines)
{
    lex::tokens_description desc;

    lex::token_definition<std::string> identifier_token;
    lex::token_definition<std::string> symbol_token;
    lex::token_definition<std::string> string_token;
    lex::token_definition<std::string> character_token;
    lex::token_definition<reaver::int128_t> bin_token;
    lex::token_definition<reaver::int128_t> dec_token;
    lex::token_definition<reaver::int128_t> hex_token;

    desc.add(identifier_token)(symbol_token)(string_token)(character_token)(bin_token)(dec_token)(hex_token);

    auto identifier = par::token(identifier_token);
    auto symbol = par::token(symbol_token);
    auto string = par::token(string_token);
    auto character = par::token(character_token);
    par::rule<reaver::int128_t> number = par::token(bin_token) | par::token(dec_token) | par::token(hex_token);
    auto op = symbol({ "+", "-", "*", "/", "%", "&", "^", "|", "<<", ">>" });

    auto directive = identifier({ "org", "extern", "global", "section" };
    auto label = identifier >> symbol({ ":" });
    auto prefix = identifier({ "rep", "repe", /* TODO */ "lock" });
    auto register16 = identifier({ ... });
    auto register32 = identifier({ ... });
    auto register64 = identifier({ ... });
    auto size_override = identifier({ "byte", "word", "dword", "qword" });
    auto address16 = symbol({ "[" }) >> (number | register16 | identifier) >> *(op >> (number | register16 | identifier)) >> symbol({ "]" });
    auto address32 = symbol({ "[" }) >> (number | register32 | identifier) >> *(op >> (number | register32 | identifier)) >> symbol({ "]" });
    auto address64 = symbol({ "[" }) >> (number | register64 | identifier) >> *(op >> (number | register64 | identifier)) >> symbol({ "]" });

    reaver::assembler::ast ret;

    for (uint64_t i = 0; i < lines.size(); ++i)
    {
        auto tokens = _tokenize(lines[i]);

        if (tokens.size() && tokens[0]->is_label())
        {
            ret.labels.push_back(std::make_pair(tokens[0]->body(), i));
            tokens.erase(tokens.begin());
        }

//        ret.lines.emplace_back(tokens);
    }

    return ret;
}

std::vector<std::shared_ptr<reaver::assembler::token>> reaver::assembler::parser::_tokenize(const reaver::assembler::line & line)
{
    std::vector<std::shared_ptr<token>> ret;

    // another huge like ass function
    for (auto & value : directive::values)
    {
        if (line->substr(0, value.size() + 1) == value + ' ')
        {
            dlog() << value << " detected";

/*            if (value == "bits")
            {
                ret.emplace_back(new bits_directive(line->substr(value.size())));
            }

            else if (value == "org")
            {
                ret.emplace_back(new org_directive(line->substr(value.size())));
            }

            else if (value == "extern")
            {
                ret.emplace_back(new extern_directive(line->substr(value.size())));
            }

            else if (value == "global")
            {
                ret.emplace_back(new global_directive(line->substr(value.size())));
            }

            else if (value == "section")
            {
                ret.emplace_back(new section_directive(line->substr(value.size())));
            }*/
        }
    }

    return {};
}
