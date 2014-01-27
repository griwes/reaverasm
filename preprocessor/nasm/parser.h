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

#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <iostream>

namespace reaver
{
    namespace assembler
    {
        namespace nasm_pp_grammar
        {
            using namespace boost::spirit::x3;

            const rule<class identifier, std::string> identifier("identifier");
            const rule<class integer, boost::multiprecision::cpp_int> integer("integer");

//            const auto identifier_def = char_("a-zA-Z_@$") >> *char_("a-zA_Z_@$0-9");
            const auto integer_def = int_;

            void foo()
            {
                boost::multiprecision::cpp_int i;
                char foo[] = "basd";
                auto b = std::begin(foo);
                integer_def.parse(b, std::end(foo), unused, i);
                std::cout << i << std::endl;
            }
        }
    }
}

int main()
{
    reaver::assembler::nasm_pp_grammar::foo();
}
