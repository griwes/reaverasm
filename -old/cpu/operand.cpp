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

#include <cpu/operand.h>
#include <parser/helpers.h>
#include <cpu/register.h>
#include <cpu/overrides.h>
#include <cpu/address.h>

reaver::assembler::operand::operand(integer i) : _type{ _integer }, _ptr{ std::make_shared<integer>(std::move(i)) }
{
}

reaver::assembler::operand::operand(cpu_register reg) : _type{ _register }, _ptr{ std::make_shared<cpu_register>
    (std::move(reg)) }
{
}

reaver::assembler::operand::operand(size_overriden_identifier l) : _type{ _label }, _ptr{ std::make_shared
    <size_overriden_identifier>(std::move(l)) }
{
}

reaver::assembler::operand::operand(effective_address addr) : _type{ _address }, _ptr{ std::make_shared<effective_address>
    (std::move(addr)) }
{
}
