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

#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <memory>

#include <reaver/logger.h>
#include <reaver/target.h>

namespace reaver
{
    namespace assembler
    {
        class define;

        struct file
        {
            file(file &&) = default;

            file(std::string n, std::ifstream && str) : name{ std::move(n) }, stream{ std::move(str) }
            {
            }

            std::string name;
            std::ifstream stream;
        };

        class frontend
        {
        public:
            virtual ~frontend() {}

            virtual bool preprocess_only() const = 0;
            virtual bool assemble_only() const = 0;

            virtual std::string preprocessor() const = 0;
            virtual std::string syntax() const = 0;
            virtual ::reaver::target::triple target() const = 0;
            virtual std::string format() const = 0;

            virtual std::istream & input() const = 0;
            virtual std::ostream & output() const = 0;

            virtual std::string input_name() const = 0;
            virtual std::vector<file> & default_includes() const = 0;

            virtual file open_file(std::string) const = 0;

            virtual const std::map<std::string, std::shared_ptr<define>> & defines() const = 0;

            virtual logger::level warning_level() const = 0;
        };
    }
}
