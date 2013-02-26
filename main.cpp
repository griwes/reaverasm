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

#include <string>
#include <fstream>
#include <vector>
#include <memory>

namespace reaver
{
    namespace assembler
    {
        enum class format
        {
            binary
        };

        class macro
        {
        };

        class assembler
        {
        public:
            void read_file(std::fstream &);
            void preprocess(const std::vector<std::unique_ptr<reaver::assembler::macro>> &);
            void parse();
            void generate(std::fstream &, bool, format);
        };
    }
}

int main(int argc, char ** argv)
{
    std::string input_name = "main.asm";
    std::string output_name = "bin";
    bool link = true;
    reaver::assembler::format format = reaver::assembler::format::binary;
    std::vector<std::unique_ptr<reaver::assembler::macro>> commandline_macros;

    std::fstream input_file(input_name, std::ios::in | std::ios::binary);
    std::fstream output_file(output_name, std::ios::out | std::ios::binary);

    reaver::assembler::assembler assembler;
    assembler.read_file(input_file);
    assembler.preprocess(commandline_macros);
    assembler.parse();
    assembler.generate(output_file, link, format);
}
