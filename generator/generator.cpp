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

#include <reaver/target.h>
#include <reaver/error.h>

#include <generator/generator.h>
#include <generator/none/none.h>
#include <generator/intel/intel.h>

using reaver::style::colors;
using reaver::style::styles;

using namespace reaver::target;

std::unique_ptr<reaver::assembler::generator> reaver::assembler::create_generator(const reaver::assembler::frontend & front,
    const reaver::assembler::parser & parser, reaver::error_engine & engine)
{
    if (front.preprocess_only())
    {
        return std::unique_ptr<generator>{ new none_generator{ parser } };
    }

    if (front.target().arch() >= arch::i386 && front.target().arch() <= arch::x86_64)
    {
        return std::unique_ptr<generator>{ new intel_generator{ front, parser, engine } };
    }

    engine.push(exception(error) << "unsupported architecture requested: " << style::style(colors::bgray, colors::def, styles::bold)
        << front.target().arch_string() << style::style() << ".");
    throw std::move(engine);
}
