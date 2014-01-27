/**
 * Reaver Project Assembler License
 *
 * Copyright © 2013-2014 Michał "Griwes" Dominiak
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
 **/

#include <reaver/logger.h>

#include "frontend/console.h"
#include "preprocessor/preprocessor.h"
#include "parser/parser.h"
#include "generator/generator.h"
#include "output/output.h"

using namespace reaver::logger;

int main(int argc, char ** argv) try
{
    reaver::error_engine engine;

    reaver::assembler::console_frontend frontend{ argc, argv, engine };
    auto preprocessor = reaver::assembler::create_preprocessor(frontend, engine);
    auto parser = reaver::assembler::create_parser(frontend, engine);
    auto generator = reaver::assembler::create_generator(frontend, engine);
    auto output = reaver::assembler::create_output(frontend, engine);

    auto preprocessed = (*preprocessor)();
    auto parsed = (*parser)(preprocessed);
    auto generated = (*generator)(parsed);
    (*output)(generated);

    if (engine.size())
    {
        engine.print(dlog);
    }
}

catch (reaver::exception & e)
{
    e.print(dlog);

    if (e.level() == crash)
    {
        return 2;
    }

    return 1;
}

catch (std::exception & e)
{
    dlog(crash) << e.what();

    return 2;
}
