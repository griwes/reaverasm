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

#include <output/output.h>
#include <output/elf32/elf32.h>
#include <output/elf64/elf64.h>
#include <output/text/text.h>

std::unique_ptr<reaver::assembler::output> reaver::assembler::create_output(const reaver::assembler::frontend & front,
    const reaver::assembler::generator & gen, reaver::error_engine & engine)
{
    if (front.preprocess_only())
    {
        return std::unique_ptr<output>{ new text_output{ front, gen, engine } };
    }

    if (front.assemble_only())
    {
        if (front.format() == "elf32")
        {
            return std::unique_ptr<output>{ new elf32_output{ front, gen, engine } };
        }

        if (front.format() == "elf64")
        {
            return std::unique_ptr<output>{ new elf64_output{ front, gen, engine } };
        }
    }

    engine.push(exception(crash) << "linker plugin not implemented yet, use -s to disable it.");
    throw std::move(engine);
//    std::unique_ptr<linker::frontend> linker_front{ new linker::assembler_frontend{ front, engine } };
//    return std::unique_ptr<output>{ new linker_output{ linker_front, gen, engine } };
}
