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

#include <reaver/logger.h>
#include <reaver/exception.h>
#include <reaver/error.h>

#include <frontend/console.h>
#include <preprocessor/preprocessor.h>
#include <parser/parser.h>
#include <generator/generator.h>
#include <output/output.h>

using namespace reaver::logger;

namespace reaver
{
    namespace assembler
    {
        const char * version_string = "Reaver Project Assembler v0.0.1 dev\nCopyright (C) 2013 Reaver Project Team\n";
    }
}

int main(int argc, char ** argv)
{
    try
    {
        reaver::error_engine engine;

        reaver::assembler::console_frontend frontend{ argc, argv, engine };
        std::unique_ptr<reaver::assembler::preprocessor> preprocessor = reaver::assembler::create_preprocessor(frontend, engine);
        std::unique_ptr<reaver::assembler::parser> parser = reaver::assembler::create_parser(frontend, *preprocessor, engine);
        std::unique_ptr<reaver::assembler::generator> generator = reaver::assembler::create_generator(frontend, *parser, engine);
        std::unique_ptr<reaver::assembler::output> output = reaver::assembler::create_output(frontend, *generator, engine);

        (*output)();
    }

    catch (reaver::exception & e)
    {
        e.print(dlog);

        if (e.level() == crash)
        {
            return -2;
        }

        return -1;
    }

    catch (std::exception & e)
    {
        dlog(error) << e.what();
        return -1;
    }
}
