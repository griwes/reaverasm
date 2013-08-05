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

#include <vector>
#include <string>
#include <utility>
#include <reaver/lexer.h>

#include <utils/include_chain.h>

namespace reaver
{
    namespace assembler
    {
        class define
        {
        public:
            define()
            {
            }

            define(std::string name, std::string definition, std::shared_ptr<utils::include_chain> include_chain)
                : _name{ std::move(name) }, _body{ std::move(definition) }, _inc_chain{ std::move(include_chain) }
            {
            }

            define(std::string name, std::vector<std::string> params, std::string definition, std::shared_ptr<utils::include_chain>
                include_chain) : _name{ std::move(name) }, _body{ std::move(definition) }, _inc_chain{ std::move(include_chain) },
                _params{ std::move(params) }
            {
            }

            define(std::string name, std::vector<lexer::token> tokens, std::shared_ptr<utils::include_chain> include_chain)
                : _name{ std::move(name) }, _inc_chain{ std::move(include_chain) }, _tokens{ std::move(tokens) }
            {
                for (const auto & x : _tokens)
                {
                    _body.append(x.as<std::string>());
                }
            }

            define(std::string name, std::vector<std::string> params, std::vector<lexer::token> tokens, std::shared_ptr<
                utils::include_chain> include_chain) : _name{ std::move(name) }, _inc_chain{ std::move(include_chain) },
                _params{ std::move(params) }, _tokens{ std::move(tokens) }
            {
                for (const auto & x : _tokens)
                {
                    _body.append(x.as<std::string>());
                }
            }

            std::string name() const
            {
                return _name;
            }

            std::string definition() const
            {
                return _body;
            }

            std::shared_ptr<utils::include_chain> source() const
            {
                return _inc_chain;
            }

            std::vector<std::string> parameters() const
            {
                return _params;
            }

            template<typename Lexer>
            const std::vector<lexer::token> & tokens(const Lexer & lex)
            {
                if (_tokens.empty())
                {
                    _tokens = lexer::tokenize(_body, lex.desc);
                }

                return _tokens;
            }

        private:
            std::string _name;
            std::string _body;
            std::shared_ptr<utils::include_chain> _inc_chain;
            std::vector<std::string> _params;
            std::vector<lexer::token> _tokens;
        };
    }
}
