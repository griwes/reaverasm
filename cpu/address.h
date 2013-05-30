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
            effective_address(boost::optional<cpu_register> seg, operand first, std::vector<std::tuple<std::string,
                operand>> v) : _segment{ std::move(seg) }
            {
                _size = first.is_register() ? first.size() : 0;
                bool bits32 = false;

                for (const auto & x : v)
                {
                    if (std::get<1>(x).is_register())
                    {
                        if (!_size)
                        {
                            _size = std::get<1>(x).size();
                        }

                        else
                        {
                            if (_size != std::get<1>(x).size())
                            {
                                throw "registers of different sizes used in a single effective address.";
                            }
                        }
                    }

                    else if (std::get<1>(x).is_label())
                    {
                        if (std::get<1>(x).size() == 32)
                        {
                            bits32 = true;
                        }
                    }
                }

                if (bits32 && _size == 16)
                {
                    throw "invaid effective address.";
                }

                if (std::count_if(v.begin(), v.end(), [](const std::tuple<std::string, operand> & op)
                {
                    return std::get<1>(op).is_register();
                }) + first.is_register() > 2)
                {
                    throw "invalid effective address.";
                }

                if (first.is_integer() || first.is_label())
                {
                    _disp = first;
                }

                else
                {
                    v.insert(v.begin(), std::make_tuple("", first.get_register()));
                }

                if (_size == 16)
                {
                    if (std::find_if(v.begin(), v.end(), [](const std::tuple<std::string, operand> & op)
                        {
                            return std::get<0>(op) == "*";
                        }) != v.end())
                    {
                        throw "invalid effective address.";
                    }

                    for (const auto & x : v)
                    {
                        if (std::get<1>(x).is_integer() || std::get<1>(x).is_label())
                        {
                            if (!_disp)
                            {
                                auto integer = std::get<1>(x).get_integer();

                                if (std::get<0>(x) == "-")
                                {
                                    integer.positive = false;
                                }

                                _disp = integer;
                            }

                            else
                            {
                                throw "address too complex (or you've just hit a current implementation barrier for address complexity).";
                            }
                        }

                        else
                        {
                            if (_base)
                            {
                                if (std::get<1>(x).name() == "si" || std::get<1>(x).name() == "di")
                                {
                                    _index = std::get<1>(x).get_register();
                                    _scale = 1;
                                }

                                else if ((std::get<1>(x).name() == "bx" || std::get<1>(x).name() == "bp") && (_base->register_name == "si"
                                    || _base->register_name == "di"))
                                {
                                    _index = std::get<1>(x).get_register();
                                    std::swap(_index, _base);
                                    _scale = 1;
                                }

                                else
                                {
                                    throw "invalid index register.";
                                }
                            }

                            else if (std::get<1>(x).name() == "si" || std::get<1>(x).name() == "di" || std::get<1>(x).name()
                                == "bp" || std::get<1>(x).name() == "bx")
                            {
                                _base = std::get<1>(x).get_register();
                            }
                        }
                    }
                }

                else
                {
                    if (std::find_if(v.begin(), v.end(), [](const std::tuple<std::string, operand> & op)
                        {
                            return std::get<0>(op) == "*";
                        }) != v.end())
                    {
                        // TODO:
                        throw "index register scale not yet implemented.";
                    }

                    for (const auto & x : v)
                    {
                        if (std::get<1>(x).is_integer() || std::get<1>(x).is_label())
                        {
                            if (!_disp)
                            {
                                auto integer = std::get<1>(x).get_integer();

                                if (std::get<0>(x) == "-")
                                {
                                    integer.positive = false;
                                }

                                _disp = integer;
                            }

                            else
                            {
                                throw "address too complex (or you've just hit a current implementation barrier for address complexity).";
                            }
                        }

                        else
                        {
                            if (!_base)
                            {
                                _base = std::get<1>(x).get_register();
                            }

                            else
                            {
                                _index = std::get<1>(x).get_register();
                                _scale = 1;
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

            uint64_t size() const
            {
                return _size;
            }

        private:
            boost::optional<cpu_register> _segment;

            boost::optional<cpu_register> _base;
            boost::optional<cpu_register> _index;
            uint64_t _scale = 0;
            boost::optional<operand> _disp;

            uint64_t _size = 0;
        };
    }
}
