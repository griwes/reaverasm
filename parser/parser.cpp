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
//  12) address (either single integer literal, or things like [rax + 4 * rbx]
//  13) operator, like +, -, *, /, <<, >>, &, ^, |
//  14) comma (is not returned in that vector of tokens)

reaver::assembler::ast reaver::assembler::parser::parse(const std::vector<reaver::assembler::line> & lines)
{
    reaver::assembler::ast ret;

    for (auto line : lines)
    {
        auto tokens = _tokenize(line);

        ret.add(tokens);
    }
}
