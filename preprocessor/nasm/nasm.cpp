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

#include "nasm.h"

std::vector<reaver::assembler::line> reaver::assembler::nasm_preprocessor::operator()() const
{
    _include_stream(_front.input(), std::make_shared<utils::include_chain>(_front.input_name()));
    return {};
}

void reaver::assembler::nasm_preprocessor::_include_stream(std::istream & is, std::shared_ptr<reaver::assembler::utils::include_chain> ic) const
{
    std::string buffer;
    std::size_t current_line = 1;

    auto chain = [&](){
        auto i = std::make_shared<utils::include_chain>(*ic);
        i->line = current_line;
        return i;
    };

    while (std::getline(is, buffer))
    {
        while (buffer.back() == '\\')
        {
            std::string b;

            if (!std::getline(is,  b))
            {
                _engine.push({
                    chain()->exception(buffer.size()),
                    exception(logger::error) << "invalid `\\` at the end of file."
                });
                buffer.pop_back();
            }

            buffer.append(b);
            ++current_line;
        }
    }
}
