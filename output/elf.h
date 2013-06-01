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

#include <map>
#include <string>
#include <fstream>

#include <output/section.h>

namespace reaver
{
    namespace assembler
    {
        namespace elf64
        {
            struct header
            {
                uint8_t ident[16] = { '\x7f', 'E', 'L', 'F', 2, 1, 0, 0 };
                uint16_t type = 1;
                uint16_t machine = 62;
                uint32_t version = 1;
                uint64_t entry = 0;
                uint64_t program_header_offset = 0;
                uint64_t section_header_offset = 0;
                uint32_t flags = 0;
                uint16_t header_size = 64;
                uint16_t program_header_entry_size = 0;
                uint16_t program_header_entry_count = 0;
                uint16_t section_header_entry_size = 64;
                uint16_t section_header_entry_count = 1;
                uint16_t section_name_table_index = 1;
            } __attribute__((__packed__));

            enum
            {
                mag0,
                mag1,
                mag2,
                mag3,
                file_class,
                data,
                version,
                osabi,
                abi_version,
                padding,
                size = 16
            };

            struct section_header
            {
                uint32_t name;
                uint32_t type;
                uint64_t flags;
                uint64_t virtual_address;
                uint64_t offset;
                uint64_t size;
                uint32_t link;
                uint32_t info;
                uint64_t address_alignment;
                uint64_t entries_size;
            } __attribute__((__packed__));

            struct symbol
            {
                uint32_t name;
                uint8_t info;
                uint8_t reserved;
                uint16_t section_table_index;
                uint64_t value;
                uint64_t size;
            } __attribute__((__packed__));

            struct relocation
            {
                uint64_t offset;
                uint64_t info;
            } __attribute__((__packed__));

            struct relocation_addend
            {
                uint64_t offset;
                uint64_t info;
                int64_t addend;
            } __attribute__((__packed__));

            struct program_header
            {
                uint32_t type;
                uint32_t flags;
                uint64_t offset;
                uint64_t virtual_address;
                uint64_t reserved_physical_address;
                uint64_t size_in_file;
                uint64_t size_in_memory;
                uint64_t alignment;
            } __attribute__((__packed__));
        }

        class elf_output
        {
        public:
            void output(const std::map<std::string, section> & sections, std::ostream & out)
            {
                elf64::header header;
                out.write(reinterpret_cast<char *>(&header), sizeof(header));

                section shstrtab{ ".shstrtab" };
                section strtab{ ".strtab" };
                section symbtab{ ".symbtab" };

                std::vector<elf64::section_header> section_headers;
                section_headers.emplace_back();
            }
        };
    }
}
