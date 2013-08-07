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

#include <reaver/error.h>
#include <reaver/target.h>

#include <parser/parser.h>
#include <parser/intel/intel.h>
#include <parser/none/none.h>

using reaver::style::colors;
using reaver::style::styles;

using namespace reaver::target;

namespace
{
    void _mismatch(const reaver::assembler::frontend & front, reaver::error_engine & engine)
    {
        using reaver::exception;

        engine.push(exception(error) << "syntax `" << front.syntax() << " is not allowed for target " << front.target() << "`.");
        throw std::move(engine);
    }
}

std::unique_ptr<reaver::assembler::parser> reaver::assembler::create_parser(const reaver::assembler::frontend & front,
    reaver::assembler::preprocessor & ppc, error_engine & engine)
{
    if (front.preprocess_only())
    {
        return std::unique_ptr<parser>{ new none_parser{ ppc } };
    }

    if (front.syntax() == "intel")
    {
        if (front.target().arch() < arch::i386 || front.target().arch() > arch::x86_64)
        {
            _mismatch(front, engine);
        }

        return std::unique_ptr<parser>{ new intel_parser{ front, ppc, engine } };
    }

    engine.push(exception(error) << "not supported syntax selected: `" << front.syntax() << "`.");
    throw std::move(engine);
}
