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

#include <frontend.h>

namespace
{
    const char * version_string = "Reaver Project Assembler v0.0.1\nCopyright (C) 2013 Reaver Project Team\n";
}

reaver::assembler::frontend::frontend(int argc, char ** argv)
{
    boost::program_options::options_description general("General options");
    general.add_options()
        ("help,h", "print this message")
        ("version,v", "prints version information");

    boost::program_options::options_description config("Configuration");
    config.add_options()
        ("input", boost::program_options::value<std::string>(), "specify input file")
        ("output,o", boost::program_options::value<std::string>()->default_value("output"), "specify output file");

    boost::program_options::options_description hidden("Hidden");
    boost::program_options::positional_options_description pod;
    pod.add("input", 1);

    boost::program_options::options_description options;
    options.add(general).add(config).add(hidden);

    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pod)
        .run(), _variables);

    boost::program_options::notify(_variables);

    if (_variables.count("help"))
    {
        std::cout << version_string << '\n';

        std::cout << general << std::endl << config;

        std::exit(0);
    }

    if (_variables.count("version"))
    {
        std::cout << version_string;
        std::cout << "Distributed under modified zlib license.\n\n";

        std::cout << "RAsm, or Reaver Assembler, is part of Reaver Project - http://reaver-project.org/,\n";
        std::cout << "Development of this part of Reaver Project started as second semester programming\n";
        std::cout << "project of Michal \"Griwes\" Dominiak.\n";
    }
}
