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
                uint8_t ident[16] = { '\x7f', 'E', 'L', 'F', 2, 1, 1, 0, 0 };
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
                uint16_t section_name_table_index = 0;
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
                uint64_t alignment;
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
            void output(const std::map<std::string, section> & sections, std::set<std::string> externs, std::set<std::string>
                globals, std::ostream & out)
            {
                elf64::header header;

                section shstrtab{ ".shstrtab" };
                section strtab{ ".strtab" };
                section symtab{ ".symtab" };

                uint64_t offset = sizeof(header);

                std::map<std::string, uint64_t> section_name_offsets;
                std::map<std::string, uint64_t> symbol_string_offset;

                std::map<std::string, uint64_t> section_index;

                shstrtab.push(0);
                strtab.push(0);

                section_name_offsets[shstrtab.name()] = shstrtab.size();
                for (const auto & x : shstrtab.name())
                {
                    shstrtab.push(x);
                }
                shstrtab.push(0);

                section_name_offsets[strtab.name()] = shstrtab.size();
                for (const auto & x : strtab.name())
                {
                    shstrtab.push(x);
                }
                shstrtab.push(0);

                section_name_offsets[symtab.name()] = shstrtab.size();
                for (const auto & x : symtab.name())
                {
                    shstrtab.push(x);
                }
                shstrtab.push(0);

                for (const auto & section : sections)
                {
                    section_name_offsets[section.first] = shstrtab.size();
                    for (const auto & x : section.first)
                    {
                        shstrtab.push(x);
                    }
                    shstrtab.push(0);

                    for (const auto & symbol : section.second.symbols())
                    {
                        if (globals.find(symbol.first) != globals.end())
                        {
                            symbol_string_offset[symbol.first] = strtab.size();
                            for (const auto & x : symbol.first)
                            {
                                strtab.push(x);
                            }
                            strtab.push(0);
                        }
                    }
                }

                for (const auto & ext : externs)
                {
                    if (symbol_string_offset.find(ext) == symbol_string_offset.end())
                    {
                        symbol_string_offset[ext] = strtab.size();
                        for (const auto & x : ext)
                        {
                            strtab.push(x);
                        }
                        strtab.push(0);
                    }
                }

                for (uint64_t i = 0; i < sizeof(elf64::symbol); ++i)
                {
                    symtab.push(0);
                }

                std::vector<elf64::section_header> section_headers;
                section_headers.emplace_back();

                auto add_section = [&](const section & section)
                {
                    if (!section.size())
                    {
                        return;
                    }

                    const std::string name = section.name();

                    elf64::section_header head{};

                    head.name = section_name_offsets[name];
                    head.offset = offset;
                    head.size = section.size();
                    offset += section.size();

                    head.link = 0;
                    head.info = 0;

                    if (name == ".bss")
                    {
                        head.type = 8;
                        head.flags = 0x1 | 0x2;
                    }

                    else if (name == ".data")
                    {
                        head.type = 1;
                        head.flags = 0x1 | 0x2;
                    }

                    else if (name == ".rodata")
                    {
                        head.type = 1;
                        head.flags = 0x2;
                    }

                    else if (name == ".text")
                    {
                        head.type = 1;
                        head.flags = 0x2 | 0x4;
                    }

                    else if (name == ".shstrtab" || name == ".strtab")
                    {
                        head.type = 3;
                    }

                    else if (name == ".symtab")
                    {
                        head.type = 2;
                        head.link = 2;
                    }

                    else if (name.substr(0, 5) == ".rela")
                    {
                        head.type = 4;
                        head.link = 3;
                        head.info = section_index[name.substr(6)];
                        head.size = sizeof(elf64::relocation_addend);
                    }

                    else
                    {
                        std::cout << "unsupported section type: " << name << std::endl;
                        throw std::exception{};
                    }

                    head.alignment = 16;

                    section_headers.emplace_back(std::move(head));

                    ++header.section_header_entry_count;
                };

                std::map<std::string, std::pair<uint64_t, uint64_t>> symbols;

                uint64_t i = 0;
                for (const auto & x : sections)
                {
                    ++i;

                    for (const auto & y : x.second.symbols())
                    {
                        if (symbols.find(y.first) != symbols.end())
                        {
                            std::cout << "multiple definitions of a symbol." << std::endl;
                            throw std::exception{};
                        }

                        elf64::symbol symb{};
                        symb.info = 0;

                        if (externs.find(y.first) != externs.end() || globals.find(y.first) != globals.end())
                        {
                            symb.info |= 1 << 4;
                        }

                        symb.name = symbol_string_offset[y.first];
                        symb.section_table_index = i;
                        symb.value = y.second;

                        for (uint64_t i = 0; i < sizeof(symb); ++i)
                        {
                            symtab.push(*(reinterpret_cast<const char *>(&symb) + i));
                        }

                        symbols[y.first] = std::make_pair(symbols.size(), y.second);
                    }
                }

                for (const auto & x : externs)
                {
                    if (symbols.find(x) == symbols.end())
                    {
                        elf64::symbol symb{};
                        symb.info = 1 << 4;
                        symb.name = symbol_string_offset[x];

                        for (uint64_t i = 0; i < sizeof(symb); ++i)
                        {
                            symtab.push(*(reinterpret_cast<const char *>(&symb) + i));
                        }

                        symbols[x] = std::make_pair(symbols.size(), 0);
                    }
                }

                std::vector<section> relocation_sections;

                for (const auto & x : sections)
                {
                    auto relocations = x.second.relocations();

                    if (relocations.size())
                    {
                        section rela{ ".rela" + x.first };

                        section_name_offsets[rela.name()] = shstrtab.size();
                        for (const auto & x : rela.name())
                        {
                            shstrtab.push(x);
                        }
                        shstrtab.push(0);

                        for (const auto & reloc : relocations)
                        {
                            // this here is ugly, TODO: refactor
                            elf64::relocation_addend rel{};
                            rel.offset = reloc.offset;

                            if (symbols.find(reloc.symbol) != symbols.end())
                            {
                                rel.info = symbols[reloc.symbol].first << 32;
                                rel.addend = symbols[reloc.symbol].second;
                            }

                            else
                            {
                                rel.info = symbols[reloc.symbol].first << 32;
                                rel.addend = -4;
                            }

                            rel.info |= 1 + reloc.pc_relative;

                            for (uint64_t i = 0; i < sizeof(rel); ++i)
                            {
                                rela.push(*(reinterpret_cast<const char *>(&rel) + i));
                            }
                        }

                        relocation_sections.emplace_back(std::move(rela));
                    }
                }

                add_section(shstrtab);
                add_section(strtab);
                add_section(symtab);

                header.section_name_table_index = 1;

                for (const auto & x : sections)
                {
                    add_section(x.second);
                }

                for (const auto & x : relocation_sections)
                {
                    add_section(x);
                }

                header.section_header_offset = offset + (8 - offset % 8);

                out.write(reinterpret_cast<const char *>(&header), sizeof(header));

                if (shstrtab.size())
                {
                    out.write(reinterpret_cast<const char *>(shstrtab.blob().data()), shstrtab.size());
                }

                if (strtab.size())
                {
                    out.write(reinterpret_cast<const char *>(strtab.blob().data()), strtab.size());
                }

                if (symtab.size())
                {
                    out.write(reinterpret_cast<const char *>(symtab.blob().data()), symtab.size());
                }

                for (const auto & x : sections)
                {
                    if (x.second.size())
                    {
                        out.write(reinterpret_cast<const char *>(x.second.blob().data()), x.second.size());
                    }
                }

                for (const auto & x : relocation_sections)
                {
                    if (x.size())
                    {
                        out.write(reinterpret_cast<const char *>(x.blob().data()), x.size());
                    }
                }

                while (offset % 8)
                {
                    ++offset;
                    out.write("\0", 1);
                }

                for (const auto & x : section_headers)
                {
                    out.write(reinterpret_cast<const char *>(&x), sizeof(x));
                }

                out.flush();
            }
        };
    }
}
