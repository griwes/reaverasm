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

#include <parser/parser.h>

reaver::assembler::parser::parser(reaver::assembler::frontend & front) : _frontend(front)
{
}

// parser tokenizes into:
//  1) directive, like "org", "extern", "global", "org", "section" and alike
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

std::vector<std::unique_ptr<reaver::assembler::token>> reaver::assembler::parser::_tokenize(const reaver::assembler::line &)
{
    return {};
}
