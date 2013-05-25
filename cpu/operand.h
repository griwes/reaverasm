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

#include <memory>

#include <parser/helpers.h>
#include <cpu/codepoint.h>

namespace reaver
{
    namespace assembler
    {
        class operand
        {
        public:
            operand(const integer &);
            operand(const cpu_register &);
            operand(const size_overriden_identifier &);
            operand(const effective_address &);

            std::vector<codepoint> encode(uint64_t) const
            {
                throw "NIY";
            }

            bool is_register() const
            {
                return _type == _register;
            }

            bool is_integer() const
            {
                return _type == _integer;
            }

            bool is_label() const
            {
                return _type == _label;
            }

            bool is_address() const
            {
                return _type == _address;
            }

            const cpu_register & get_register() const
            {
                return _ptr->get_register();
            }

            const cpu_register & get_segment_override() const
            {
                return _ptr->get_segment_override();
            }

            bool has_segment_override() const
            {
                return _type == _address && _ptr->has_segment_override();
            }

            const effective_address & get_address() const
            {
                return _ptr->get_address();
            }

            uint64_t size() const
            {
                return _ptr->size();
            }

            std::string name() const
            {
                return _ptr->name();
            }

        private:
            enum
            {
                _register,
                _integer,
                _label,
                _address
            } _type;

            std::shared_ptr<operand_base> _ptr;
        };
    }
}
