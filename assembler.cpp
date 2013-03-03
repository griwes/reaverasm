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

#include <assembler.h>
#include <preprocessor/preprocessor.h>

reaver::assembler::assembler::assembler()
{
}

reaver::assembler::assembler::~assembler()
{
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
            if (_iter != std::istreambuf_iterator<CharT>() && *_iter == ';')
            {
                while (_iter != std::istreambuf_iterator<CharT>() && *_iter != '\n')
                {
                    ++_iter;

                    if (_iter != std::istreambuf_iterator<CharT>() && *_iter == '\\')
                    {
                        ++_iter;

                        if (*this == _comment_remove_iterator())
                        {
                            std::cout << "Syntax error: \\ at the end of the file" << std::endl;
                            exit(0);
                        }
                    }
                }
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

            if (_iter != std::istreambuf_iterator<CharT>() && *_iter == ';')
            {
                while (_iter != std::istreambuf_iterator<CharT>() && *_iter != '\n')
                {
                    ++_iter;

                    if (_iter != std::istreambuf_iterator<CharT>() && *_iter == '\\')
                    {
                        ++_iter;

                        if (*this == _comment_remove_iterator())
                        {
                            std::cout << "Syntax error: \\ at the end of the file" << std::endl;
                            exit(0);
                        }
                    }
                }
            }

            return *this;
        }

    private:
        std::istreambuf_iterator<CharT> _iter;
    };
}

void reaver::assembler::assembler::read_input(std::istream & input, std::string name)
{
    _buffer = std::string(_comment_remove_iterator<char>(input.rdbuf()), _comment_remove_iterator<char>());
    _name = name;
}

void reaver::assembler::assembler::preprocess(const std::map<std::string, std::shared_ptr<reaver::assembler::macro>> & macros)
{
    reaver::assembler::preprocessor preprocessor(macros);

    _lines = preprocessor.preprocess(_buffer, _name);
}
