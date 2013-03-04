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

#include <boost/filesystem.hpp>

#include <frontend.h>
#include <utils.h>

namespace
{
    const char * version_string = "Reaver Project Assembler v0.0.1 dev\nCopyright (C) 2013 Reaver Project Team\n";
}

namespace
{
    template<typename CharT>
    class _comment_remove_iterator
    {
    public:
        typedef std::input_iterator_tag iterator_category;

        typedef typename std::istreambuf_iterator<CharT>::value_type value_type;
        typedef typename std::istreambuf_iterator<CharT>::pointer pointer;
        typedef typename std::istreambuf_iterator<CharT>::reference reference;
        typedef typename std::istreambuf_iterator<CharT>::difference_type difference_type;

        _comment_remove_iterator()
        {
        }

        _comment_remove_iterator(std::basic_streambuf<CharT> * buf) : _iter(buf)
        {
            while (_iter != std::istreambuf_iterator<CharT>() && std::isspace(*_iter))
            {
                ++_iter;
            }

            if (_iter != std::istreambuf_iterator<CharT>() && (*_iter == ';' || _skip_next))
            {
                while (_iter != std::istreambuf_iterator<CharT>() && *_iter != '\n')
                {
                    _skip_next = false;

                    ++_iter;

                    if (_iter != std::istreambuf_iterator<CharT>() && *_iter == '\\')
                    {
                        ++_iter;

                        if (*this == _comment_remove_iterator())
                        {
                            std::cout << "Syntax error: \\ at the end of the file" << std::endl;
                            exit(0);
                        }

                        _skip_next = true;
                    }
                }
            }

            if (_iter != std::istreambuf_iterator<CharT>() && *_iter == '\n')
            {
                _skip_ws = true;
            }
        }

        bool operator==(const _comment_remove_iterator & rhs) const
        {
            return _iter == rhs._iter;
        }

        bool operator!=(const _comment_remove_iterator & rhs) const
        {
            return !(*this == rhs);
        }

        reference operator*()
        {
            return *_iter;
        }

        pointer operator->()
        {
            return _iter;
        }

        _comment_remove_iterator & operator++()
        {
            ++_iter;

            if (_skip_ws)
            {
                _skip_ws = false;

                while (_iter != std::istreambuf_iterator<CharT>() && std::isspace(*_iter) && *_iter != '\n')
                {
                    ++_iter;
                }
            }

            if (_iter != std::istreambuf_iterator<CharT>() && (*_iter == ';' || _skip_next))
            {
                while (_iter != std::istreambuf_iterator<CharT>() && *_iter != '\n')
                {
                    _skip_next = false;

                    ++_iter;

                    if (_iter != std::istreambuf_iterator<CharT>() && *_iter == '\\')
                    {
                        ++_iter;

                        if (*this == _comment_remove_iterator())
                        {
                            std::cout << "Syntax error: \\ at the end of the file" << std::endl;
                            exit(0);
                        }

                        _skip_next = true;
                    }
                }
            }

            if (_iter != std::istreambuf_iterator<CharT>() && *_iter == '\n')
            {
                _skip_ws = true;
            }

            return *this;
        }

    private:
        std::istreambuf_iterator<CharT> _iter;

        bool _skip_next = false;
        bool _skip_ws = false;
    };
}

reaver::assembler::frontend::frontend(int argc, char ** argv)
{
    boost::program_options::options_description general("General options");
    general.add_options()
        ("help,h", "print this message")
        ("version,v", "prints version information");

    boost::program_options::options_description config("Configuration");
    config.add_options()
        ("output,o", boost::program_options::value<std::string>()->default_value("output"), "specify output file")
        (",E", "preprocess only")
        (",c", "assemble only, do not link")
        ("include-dir,I", boost::program_options::value<std::vector<std::string>>(), "specify additional include directories")
        ("include,i", boost::program_options::value<std::vector<std::string>>(), "specify automatically included file");

    boost::program_options::options_description hidden("Hidden");
    hidden.add_options()
        ("input", boost::program_options::value<std::string>()->required(), "specify input file");

    boost::program_options::positional_options_description pod;
    pod.add("input", 1);

    boost::program_options::options_description options;
    options.add(general).add(config).add(hidden);

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
        std::cout << "Error: you must specify input file.\n";

        std::exit(-1);
    }

    _output.open(_variables["output"].as<std::string>(), std::ios::out | std::ios::binary);
    if (!_output)
    {
        std::cout << "Error: failed to open output file `" << _variables["output"].as<std::string>() << "`.\n";

        std::exit(-1);
    }
}

std::ostream & reaver::assembler::frontend::output() const
{
    return _output;
}

std::string reaver::assembler::frontend::input_name() const
{
    return _variables["input"].as<std::string>();
}

std::string reaver::assembler::frontend::read_file() const
{
    std::fstream input(input_name(), std::ios::in | std::ios::binary);

    if (!input)
    {
        std::cout << "Error: failed to open input file `" << input_name() << "`\n.";
        std::exit(-1);
    }

    return std::string(_comment_remove_iterator<char>(input.rdbuf()), _comment_remove_iterator<char>());
}

std::string reaver::assembler::frontend::read_file(std::string filename, std::vector<std::pair<std::string, uint64_t>> inc) const
{
    std::fstream input(_resolve_name(filename, inc), std::ios::in | std::ios::binary);

    if (!input)
    {
        print_include_chain(inc);
        std::cout << "Error: failed to open file `" << filename << "`.\n";

        std::exit(-1);
    }

    return std::string(_comment_remove_iterator<char>(input.rdbuf()), _comment_remove_iterator<char>());
}

std::string reaver::assembler::frontend::absolute_name() const
{
    return boost::filesystem::absolute(input_name()).string();
}

std::map<std::string, std::shared_ptr<reaver::assembler::macro>> reaver::assembler::frontend::macros()
{
    return std::map<std::string, std::shared_ptr<reaver::assembler::macro>>();
}

std::string reaver::assembler::frontend::_resolve_name(std::string filename, std::vector<std::pair<std::string, uint64_t>> inc) const
{
    if (boost::filesystem::path(filename).is_absolute())
    {
        if (boost::filesystem::is_regular_file(filename))
        {
            return filename;
        }

        else
        {
            print_include_chain(inc);
            std::cout << "Error: file `" << filename << "` (best match for given filename) is not a regular file.\n";

            std::exit(-1);
        }
    }

    std::vector<std::string> paths = _variables.count("include-dir") ? _variables["include-dir"].as<std::vector<std::string>>()
        : std::vector<std::string>();
    paths.insert(paths.begin(), boost::filesystem::absolute(".").string());
    paths.insert(paths.begin() + 1, boost::filesystem::path(absolute_name()).parent_path().string());

    for (auto & path : paths)
    {
        if (boost::filesystem::is_regular_file(path + "/" + filename))
        {
            return path + "/" + filename;
        }
    }

    print_include_chain(inc);
    std::cout << "Error: file `" << filename << "` not found.\n";

    std::exit(-1);

    return {};
}

bool reaver::assembler::frontend::default_includes() const
{
    return _variables.count("include");
}

std::vector< std::string > reaver::assembler::frontend::get_default_includes() const
{
    return _variables["include"].as<std::vector<std::string>>();
}
