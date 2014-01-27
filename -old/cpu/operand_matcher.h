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

#include <cpu/instruction.h>
#include "register.h"

namespace reaver
{
    namespace assembler
    {
        class operand_matcher
        {
        public:
            virtual ~operand_matcher() {}

            virtual bool operator()(const operand &) const = 0;

            virtual uint64_t size() const
            {
                throw "called size() on not-sized operand matcher; consider this an internal error.";
            }
        };

        class register_matcher : public operand_matcher
        {
        public:
            register_matcher(std::string name) : _name{ name }, _size{ cpu_register::implicit }
            {
            }

            register_matcher(cpu_register::sizes size) : _size{ size }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_register() && op.get_register().type == cpu_register::general_purpose && ((_name.empty()
                    && op.size() == _size) || op.name() == _name);
            }

            virtual uint64_t size() const
            {
                return _size;
            }

        private:
            std::string _name;
            cpu_register::sizes _size;
        };

        class immediate_value_matcher : public operand_matcher
        {
        public:
            immediate_value_matcher(cpu_register::sizes size) : _size{ size }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_label() || (op.is_integer() && op.size() <= _size);
            }

            virtual uint64_t size() const
            {
                return _size;
            }

        private:
            cpu_register::sizes _size;
        };

        class address_matcher : public operand_matcher
        {
        public:
            address_matcher()
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_address();
            }
        };

        class rm_matcher : public operand_matcher
        {
        public:
            rm_matcher(cpu_register::sizes size) : _size{ size }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_address() || (op.is_register() && op.get_register().type == cpu_register::general_purpose && op.size() <= _size);
            }

            virtual uint64_t size() const
            {
                return _size;
            }

        private:
            cpu_register::sizes _size;
        };

        class special_register_matcher : public operand_matcher
        {
        public:
            special_register_matcher(cpu_register::types t) : _type{ t }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_register() && op.get_register().type == _type;
            }

        private:
            cpu_register::types _type;
        };

        class far_pointer_matcher : public operand_matcher
        {
        public:
            far_pointer_matcher()
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_address() && op.has_segment_override();
            }
        };

/*        class moffs_matcher : public operand_matcher
        {
        public:
            moffs_matcher()
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_address() && op.has_segment_override() && !op.get_address().has_base() && !op.get_address().has_index();
            }
        };
*/
        class operand_type
        {
        public:
            template<typename T, typename = typename std::enable_if<std::is_base_of<operand_matcher, T>::value>::type>
            operand_type(T && t) : _ptr{ std::make_shared<T>(std::forward<T>(t)) }
            {
            }

            bool match(const operand & op) const
            {
                return (*_ptr)(op);
            }

            uint64_t size() const
            {
                return _ptr->size();
            }

        private:
            std::shared_ptr<operand_matcher> _ptr;
        };
    }
}
