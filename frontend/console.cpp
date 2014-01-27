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

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <reaver/error.h>

#include "console.h"

namespace reaver
{
    namespace assembler
    {
        const char * version_string = "Reaver Project Assembler v0.0.1 dev\nCopyright (C) 2013 Reaver Project Team\n";
    }
}

using reaver::style::style;
using reaver::style::colors;
using reaver::style::styles;

using namespace reaver::target;

reaver::assembler::console_frontend::console_frontend(int argc, char ** argv, error_engine & engine)
{
    boost::program_options::options_description general("General options");
    general.add_options()
        ("help,h", "print this message")
        ("version,v", "print version information");

    boost::program_options::options_description config("Configuration");
    config.add_options()
        ("output,o", boost::program_options::value<std::string>()->default_value(""), "specify output file")
        ("preprocess-only,E", "preprocess only")
        ("assemble-only,s", "assemble only, do not link")
        ("include-dir,I", boost::program_options::value<std::vector<std::string>>(&_include_paths)->composing(), "specify additional"
            " include directories")
        ("include,i", boost::program_options::value<std::vector<std::string>>()->composing(), "specify automatically included file")
        ("preprocessor,p", boost::program_options::value<std::string>()->default_value("nasm"), "specify preprocessor "
            "to use; currently supported:\n- none \n- nasm")
        ("syntax", boost::program_options::value<std::string>()->default_value(""), "specify assembly syntax "
            "(x86 and x86_64 only); currently supported:\n- intel (default for x86 and x86_64 targets)")
        ("format,f", boost::program_options::value<std::string>()->default_value("elf64"), "specify format; currently "
            "supported:\n- binary (flat)\n- elf32\n- elf64")
        ("target,t", boost::program_options::value<std::string>()->default_value("x86_64-none-elf"), "specify target in "
            "triple format; currently supported:\n- i(X)86-none-elf\n- i(X)86-linux-elf\n- x86_64-none-elf\n- x86_64-linux-elf");

    boost::program_options::options_description errors("Error and optimization options");
    errors.add_options()
        ("Wextra", boost::program_options::value<bool>(&_wextra)->implicit_value(true), " enable additional warnings")
        ("Werror", boost::program_options::value<bool>(&_werror)->implicit_value(true), " throw errors instead of warnings")
        ("Wno-long-mode-ss-write", boost::program_options::value<bool>(&_no_ss_warning)->implicit_value(true), " disable warning"
            " about write to segment register being ignored in 64 bit mode, if the segment register is SS (i(X)86 and x86_64 only)")
        ("optimizations,O", boost::program_options::value<int>(&_opt), "set optimization level; supported levels:\n"
            "- O0 - disable all optimizations\n- O1 - enable space optimizations (default)\n- O2 - enable additional optimizations");

    boost::program_options::options_description preprocessor("Preprocessor options");
    preprocessor.add_options()
        ("D*", boost::program_options::value<std::string>()->implicit_value(""), " define names for preprocessor");

    boost::program_options::options_description hidden("Hidden");
    hidden.add_options()
        ("input", boost::program_options::value<std::string>(), "specify input file");

    boost::program_options::positional_options_description pod;
    pod.add("input", 1);

    boost::program_options::options_description options;
    options.add(config).add(hidden).add(general).add(errors).add(preprocessor);
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pod)
        .style(boost::program_options::command_line_style::allow_short
            | boost::program_options::command_line_style::allow_long
            | boost::program_options::command_line_style::allow_sticky
            | boost::program_options::command_line_style::allow_dash_for_short
            | boost::program_options::command_line_style::long_allow_next
            | boost::program_options::command_line_style::short_allow_next
            | boost::program_options::command_line_style::allow_long_disguise).run(), _variables);

    if (_variables.count("help"))
    {
        std::cout << version_string << '\n';

        std::cout << "Usage:\n";
        std::cout << "  rasm [options] <input file> [options]\n\n";

        std::stringstream ss;
        ss << general << std::endl << config << std::endl << errors << std::endl << preprocessor;
        std::string str = ss.str();
        boost::algorithm::replace_all(str, "--W", "-W");
        boost::algorithm::replace_all(str, "--D", "-D");

        std::cout << str;

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

    _input_name = _variables.count("input") ? _variables["input"].as<std::string>() : "";

    if (_input_name == "")
    {
        engine.push(exception(logger::error) << "you must specify input file.");
        throw std::move(engine);
    }

    _input.open(_input_name, std::ios::in);
    if (!_input)
    {
        engine.push(exception(logger::error) << "failed to open input file `"  << _input_name << ".");
        throw std::move(engine);
    }

    if (_variables["output"].as<std::string>() == "")
    {
        _variables.at("output").value() = boost::any{ boost::filesystem::path{ _input_name }.replace_extension(".out").string() };
    }

    _output.open(_variables["output"].as<std::string>(), std::ios::out | std::ios::binary);
    if (!_output)
    {
        engine.push(exception(logger::error) << "failed to open output file `" << _variables["output"].as<std::string>() << ".");
        throw std::move(engine);
    }

    if (_variables.count("preprocess-only"))
    {
        _prep_only = true;
    }

    if (_variables.count("assemble-only") || boost::filesystem::path(_variables["output"].as<std::string>()).extension() == ".o")
    {
        _asm_only = true;
    }

    if (_asm_only && _prep_only)
    {
        engine.push(exception(logger::error) << "-s (--assemble-only) and -E (--preprocess-only) are not allowed together.");
        throw std::move(engine);
    }

    _target = _variables["target"].as<std::string>();

    if (_target.arch() >= arch::i386 && _target.arch() <= arch::x86_64 && _variables["syntax"].as<std::string>() == "")
    {
        _variables.at("syntax").value() = boost::any{ std::string{ "intel" } };
    }

    if (_variables.count("include"))
    {
        for (const auto & x : _variables.at("include").as<std::vector<std::string>>())
        {
            _default_includes.emplace_back(open_file(x));
        }
    }

    if (_variables.count("optimizations"))
    {
        _opt = _variables.at("optimizations").as<int>();
    }

    if (_opt > 2)
    {
        engine.push(exception(logger::warning) << "not supported optimization level requested; changing to 2.");
        _opt = 2;
    }

    auto chain = std::make_shared<utils::include_chain>("<command line>");
    for (const auto & value : _variables)
    {
        if (value.first[0] == 'D')
        {
            _defines.emplace(std::make_pair(value.first.substr(1), std::make_shared<define>(value.first.substr(1), value.second.as<std::string>(),
                chain)));
        }
    }

    _include_paths.insert(_include_paths.begin(), boost::filesystem::current_path().string());
    _include_paths.insert(_include_paths.begin() + 1, boost::filesystem::absolute(_input_name).parent_path().string());
}

reaver::assembler::file reaver::assembler::console_frontend::open_file(std::string filename) const
{
    if (boost::filesystem::path(filename).is_absolute())
    {
        if (boost::filesystem::is_regular_file(filename))
        {
            std::ifstream ret{ filename, std::ios::in };

            if (!ret)
            {
                throw file_failed_to_open{ filename };
            }

            return { filename, std::move(ret) };
        }

        else
        {
            throw file_is_directory{ filename };
        }
    }

    for (auto & path : _include_paths)
    {
        if (boost::filesystem::is_regular_file(path + "/" + filename))
        {
            std::ifstream ret{ path + "/" + filename, std::ios::in };

            if (!ret)
            {
                throw file_failed_to_open{ filename };
            }

            return { (path == _include_paths[0] || path == _include_paths[1]) ? filename : path + "/" + filename, std::move(ret) };
        }
    }

    throw file_not_found{ filename };
}
