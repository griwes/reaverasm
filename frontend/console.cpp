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

#include <iostream>

#include <boost/filesystem.hpp>

#include <reaver/exception.h>

#include <frontend/console.h>

namespace reaver
{
    namespace assembler
    {
        extern const char * version_string;
    }
}

using reaver::style::style;
using reaver::style::colors;
using reaver::style::styles;

reaver::assembler::console_frontend::console_frontend(int argc, char ** argv)
{
    boost::program_options::options_description general("General options");
    general.add_options()
        ("help,h", "print this message")
        ("version,v", "print version information");

    boost::program_options::options_description config("Configuration");
    config.add_options()
        ("output,o", boost::program_options::value<std::string>()->default_value(""), "specify output file")
        (",E", boost::program_options::value<bool>(&_prep_only)->implicit_value(true), "preprocess only")
        (",c", boost::program_options::value<bool>(&_asm_only)->implicit_value(true), "assemble only, do not link")
        ("include-dir,I", boost::program_options::value<std::vector<std::string>>(), "specify additional include directories")
        ("include,i", boost::program_options::value<std::vector<std::string>>(), "specify automatically included file")
        ("preprocessor,p", boost::program_options::value<std::string>()->default_value("nasm"), "specify preprocessor "
            "to use; currently supported:\n- none \n- nasm")
        ("syntax,s", boost::program_options::value<std::string>()->default_value(""), "specify assembly syntax "
            "(x86 and x86_64 only); currently supported:\n- intel (default for x86 and x86_64 targets)")
        ("format,f", boost::program_options::value<std::string>()->default_value("elf64"), "specify format; currently "
            "supported:\n- binary (flat)\n- elf32\n- elf64")
        ("target,t", boost::program_options::value<std::string>()->default_value("x86_64-none-elf"), "specify target in "
            "triple format; currently supported:\n- x86-none-elf\n- x86_64-none-elf");

    boost::program_options::options_description hidden("Hidden");
    hidden.add_options()
        ("input", boost::program_options::value<std::string>()->required(), "specify input file");

    boost::program_options::positional_options_description pod;
    pod.add("input", 1);

    boost::program_options::options_description options;
    options.add(hidden).add(general).add(config);

    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pod)
        .style(boost::program_options::command_line_style::unix_style).run(), _variables);

    if (_variables.count("help"))
    {
        std::cout << version_string << '\n';

        std::cout << "Usage:\n";
        std::cout << "  rasm [options] <input file> [options]\n\n";

        std::cout << general << std::endl << config;

        std::exit(0);
    }

    if (_variables.count("version"))
    {
        std::cout << version_string;
        std::cout << "Distributed under modified zlib license.\n\n";

        std::cout << "RAsm, or Reaver Assembler, is part of Reaver Project - http://reaver-project.org/.\n";
        std::cout << "Development of this part of Reaver Project started as second semester programming\n";
        std::cout << "project of Michal \"Griwes\" Dominiak.\n";

        std::exit(0);
    }

    if (!_variables.count("input"))
    {
        throw exception(error) << "you must specify input file.";
    }

    _input.open(_variables["input"].as<std::string>(), std::ios::in | std::ios::binary);
    if (!_input)
    {
        throw exception(error) << "failed to open input file " << style::style(colors::bgray, colors::def, styles::bold)
            << _variables["input"].as<std::string>() << style::style() << ".";
    }

    if (_variables["output"].as<std::string>() == "")
    {
        _variables.at("output").value() = boost::any{ boost::filesystem::path{ _variables["input"].as<std::string>() }
            .replace_extension(".out").string() };
    }

    _output.open(_variables["output"].as<std::string>(), std::ios::out | std::ios::binary);
    if (!_output)
    {
        throw exception(error) << "failed to open output file " << style::style(colors::bgray, colors::def, styles::bold)
            << _variables["output"].as<std::string>() << style::style() << ".";
    }

    if (boost::filesystem::path(_variables["output"].as<std::string>()).extension() == ".o")
    {
        _asm_only = true;
    }

    std::string target = _variables["target"].as<std::string>();
    _arch = target.substr(0, target.find('_'));

    if ((_arch == "x86" || _arch == "x86_64") && _variables["syntax"].as<std::string>() == "")
    {
        _variables.at("syntax").value() = boost::any{ std::string{ "intel" } };
    }
}

std::string reaver::assembler::console_frontend::preprocessor() const
{
    return _variables["preprocessor"].as<std::string>();
}

std::string reaver::assembler::console_frontend::arch() const
{
    return _arch;
}

std::string reaver::assembler::console_frontend::syntax() const
{
    return _variables["syntax"].as<std::string>();
}

bool reaver::assembler::console_frontend::preprocess_only() const
{
    return _prep_only;
}
