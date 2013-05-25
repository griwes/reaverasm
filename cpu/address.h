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

#include <boost/variant.hpp>

#include <cpu/overrides.h>
#include <cpu/register.h>
#include <cpu/operand.h>
#include <parser/helpers.h>

#include <utils.h>

namespace reaver
{
    namespace assembler
    {
        class effective_address : public operand_base
        {
        public:
            effective_address(boost::optional<cpu_register> seg, operand first, const std::vector<std::tuple<std::string,
                operand>> & v) : _segment{ std::move(seg) }
            {
                if (v.size() > 4)
                {
                    throw "too many address operands for effective address";
                }

                uint64_t size = first.is_register() ? first.size() : 0;

                for (const auto & x : v)
                {
                    if (std::get<1>(x).is_register())
                    {
                        if (!size)
                        {
                            size = std::get<1>(x).size();
                        }

                        else
                        {
                            if (size != std::get<1>(x).size())
                            {
                                throw "registers of different sizes used in a single effective address.";
                            }
                        }
                    }
                }
            }

            virtual bool has_segment_override() const
            {
                return _segment;
            }

            virtual const cpu_register & get_segment_override() const
            {
                return *_segment;
            }

            virtual const effective_address & get_address() const
            {
                return *this;
            }

            bool has_base() const
            {
                return _base;
            }

            const cpu_register & base() const
            {
                return *_base;
            }

            bool has_index() const
            {
                return _index;
            }

            const cpu_register & index() const
            {
                return *_index;
            }

            uint64_t scale() const
            {
                return _scale;
            }

            bool has_disp() const
            {
                return _disp;
            }

            const operand & disp() const
            {
                return *_disp;
            }

        private:
            boost::optional<cpu_register> _segment;

            boost::optional<cpu_register> _base;
            boost::optional<cpu_register> _index;
            uint64_t _scale = 1;
            boost::optional<operand> _disp;
        };
    }
}
