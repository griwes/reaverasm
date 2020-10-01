/**
 * Reaver Project Assembler License
 *
 * Copyright © 2014 Michał "Griwes" Dominiak
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

#include <boost/spirit/include/lex_lexertl.hpp>

#include "intel.h"
#include "../../utils/include_chain.h"
#include "grammar.h"
#include "tokens.h"

reaver::assembler::ast reaver::assembler::intel_parser::operator()() const
{
    return _parse_stream(_front.input(), std::make_shared<utils::include_chain>(_front.input_name()));
}

reaver::assembler::ast reaver::assembler::intel_parser::_parse_stream(std::istream & is, std::shared_ptr<
    reaver::assembler::utils::include_chain> ic) const
{
    std::string buffer;
    std::size_t current_line = 0;
    std::size_t more_lines = 0;

    auto chain = [&](){
        auto i = std::make_shared<utils::include_chain>(*ic);
        i->line = current_line;
        return i;
    };

    ast ret;

    using iterator = std::string::const_iterator;
    using token_type = lex::lexertl::token<iterator, boost::mpl::vector<lex::omit, std::string>>;
    using lexer_type = lex::lexertl::lexer<token_type>;
    using skipper_type = qi::in_state_skipper<intel_tokens<lexer_type>::lexer_def>;

    intel_tokens<lexer_type> lexer;
    intel_grammar<intel_tokens<lexer_type>::iterator_type, skipper_type> grammar{ lexer, ret, chain, current_line };

    while (std::getline(is, buffer))
    {
        current_line += more_lines + 1;
        more_lines = 0;

        while (buffer.back() == '\\')
        {
            std::string b;

            buffer.pop_back();

            if (!is.eof())
            {
                is.unget();
            }

            if (is.eof() || !std::getline(is,  b))
            {
                _engine.push({
                    chain()->exception(buffer.size() + 1),
                    exception(logger::error) << "invalid `\\` at the end of file."
                  });
            }

            else
            {
                buffer.append(b);
                ++more_lines;
            }
        }

        auto begin = buffer.cbegin();
        lex::tokenize_and_phrase_parse(begin, buffer.cend(), lexer, grammar, qi::in_state("skip")[lexer.self]);
    }

    return ret;
}
