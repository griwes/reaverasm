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

#include <preprocessor/preprocessor.h>
#include <preprocessor/nasm/nasm.h>
#include <preprocessor/none/none.h>

using reaver::style::colors;
using reaver::style::styles;

std::unique_ptr<reaver::assembler::preprocessor> reaver::assembler::create_preprocessor(const reaver::assembler::frontend & front,
    error_engine & engine)
{
    if (front.preprocessor() == "none")
    {
        return std::unique_ptr<preprocessor>{ new none_preprocessor{ front } };
    }

    if (front.preprocessor() == "nasm")
    {
        return std::unique_ptr<preprocessor>{ new nasm_preprocessor{ front, engine } };
    }

    engine.push(exception(error) << "not supported preprocessor selected: " << style::style(colors::bgray, colors::def,
        styles::bold) << front.preprocessor() << style::style() << ".");
    throw std::move(engine);
}
