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

#include <parser/grammar.h>

reaver::assembler::ast reaver::assembler::parser::parse(const std::vector<reaver::assembler::line> & lines) const
{
    ast ret;

    lexer lex{};
    parser par{ lex };

    for (const auto & x : lines)
    {
        try
        {
            auto t = reaver::lexer::tokenize(*x, lex.desc);

            auto begin = t.cbegin();

            auto label_match = reaver::parser::parse(par.label_definition, begin, t.cend(), par.skip);

            if (label_match)
            {
                ret.add_label(*label_match);
            }

            {
                auto b = begin;
                auto data_match = reaver::parser::parse(par.data, b, t.cend(), par.skip);

                if (data_match)
                {
                    if (b != t.cend())
                    {
                        throw "garbage at the end of a line";
                    }

                    ret.add_data(*data_match);

                    continue;
                }
            }

            {
                auto b = begin;
                auto bits_match = reaver::parser::parse(par.bits_directive, b, t.cend(), par.skip);

                if (bits_match)
                {
                    if (b != t.cend())
                    {
                        throw "garbage at the end of a line";
                    }

                    ret.set_bitness(*bits_match);

                    continue;
                }
            }

            {
                auto b = begin;
                auto extern_match = reaver::parser::parse(par.extern_directive, b, t.cend(), par.skip);

                if (extern_match)
                {
                    if (b != t.cend())
                    {
                        throw "garbage at the end of a line";
                    }

                    ret.add_extern(*extern_match);

                    continue;
                }
            }

            {
                auto b = begin;
                auto global_match = reaver::parser::parse(par.global_directive, b, t.cend(), par.skip);

                if (global_match)
                {
                    if (b != t.cend())
                    {
                        throw "garbage at the end of a line";
                    }

                    ret.add_global(*global_match);

                    continue;
                }
            }

            {
                auto b = begin;
                auto instruction_match = reaver::parser::parse(par.assembly_instruction, b, t.cend(), par.skip);

                if (instruction_match)
                {
                    if (b != t.cend())
                    {
                        throw "garbage at the end of a line";
                    }

                    ret.add_instruction(*instruction_match);

                    continue;
                }

                throw "invalid line";
            }
        }

        catch (const char * e)
        {
            throw exception{ error, x.chain() } << e;
        }
    }

    return ret;
}
