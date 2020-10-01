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
 * Michał "Griwes" Dominiak
 *
 **/

#include "generator.h"
#include "intel/intel.h"

using reaver::style::colors;
using reaver::style::styles;

using namespace reaver::target;

std::unique_ptr<reaver::assembler::generator> reaver::assembler::create_generator(const reaver::assembler::frontend & front,
    reaver::error_engine & engine)
{
    if (front.target().arch() >= arch::i386 && front.target().arch() <= arch::x86_64)
    {
        return std::make_unique<intel_generator>(front, engine);
    }

    engine.push(exception(logger::error) << "unsupported architecture requested: `" << front.target().arch_string() << "`.");
    throw std::move(engine);
}
