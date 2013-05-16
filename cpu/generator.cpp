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

#pragma once

#include <cpu/generator.h>
#include <cpu/opcode.h>
#include <cpu/cpu.h>

namespace
{
    const reaver::assembler::opcode & _find(const reaver::assembler::instruction &, reaver::assembler::mode m)
    {

    }
}

std::vector<uint8_t> reaver::assembler::pmode_generator::generate(const reaver::assembler::instruction & i)
{
    std::vector<uint8_t> ret;

    auto opcode = _find(i, _bits32 ? bits32 : bits16);

    if (opcode.mode().find(_bits32 ? mode16 : mode32) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }
}

std::vector<uint8_t> reaver::assembler::lmode_generator::generate(const reaver::assembler::instruction & i)
{
    std::vector<uint8_t> ret;

    auto opcode = _find(i, bits64);

    if (opcode.mode().find(mode16) != opcode.mode().end())
    {
        ret.push_back(0x66);
    }
}
