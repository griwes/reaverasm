#pragma once

#include <string>
#include <boost/iterator/iterator_concepts.hpp>

namespace reaver
{
    namespace assembler
    {
        class token
        {
        public:
            virtual ~token() {}

            virtual std::string body();

            virtual bool is_label()
            {
                return false;
            }

            virtual bool is_directive()
            {
                return false;
            }

            virtual bool is_prefix()
            {
                return false;
            }

            virtual bool is_symbol_address()
            {
                return false;
            }

            virtual bool is_string()
            {
                return false;
            }

            virtual bool is_character()
            {
                return false;
            }

            virtual bool is_binary()
            {
                return false;
            }

            virtual bool is_decimal()
            {
                return false;
            }

            virtual bool is_hexadecimal()
            {
                return false;
            }

            virtual bool is_register()
            {
                return false;
            }

            virtual bool is_size_override()
            {
                return false;
            }

            virtual bool is_operator()
            {
                return false;
            }

            virtual bool is_segment_override()
            {
                return false;
            }

            virtual bool is_mnemonic()
            {
                return false;
            }

            virtual bool is_operand()
            {
                return false;
            }

            virtual bool is_address()
            {
                return false;
            }
        };

        class directive : public token
        {
        public:
            virtual ~directive() {}

            virtual bool is_directive()
            {
                return true;
            }

            static const std::vector<std::string> values;
        };

        class bits_directive : public directive
        {
        public:
            bits_directive(std::string);

            virtual ~bits_directive() {}
        };

        class label : public token
        {
        public:
            virtual ~label() {}

            virtual bool is_label()
            {
                return true;
            }
        };

        class prefix : public token
        {
        public:
            virtual ~prefix() {}

            virtual bool is_prefix()
            {
                return true;
            }
        };

        class symbol_address : public token
        {
        public:
            virtual ~symbol_address() {}

            virtual bool is_symbol_address()
            {
                return true;
            }
        };

        class string : public token
        {
        public:
            virtual ~string() {}

            virtual bool is_string()
            {
                return true;
            }
        };

        class character : public token
        {
        public:
            virtual ~character() {}

            virtual bool is_character()
            {
                return true;
            }
        };

        class binary_literal : public token
        {
        public:
            virtual ~binary_literal() {}

            virtual bool is_binary()
            {
                return true;
            }
        };

        class decimal_literal : public token
        {
        public:
            virtual ~decimal_literal() {}

            virtual bool is_decimal()
            {
                return true;
            }
        };

        class hexadecimal_literal : public token
        {
        public:
            virtual ~hexadecimal_literal() {}

            virtual bool is_hexadecimal()
            {
                return true;
            }
        };

        class cpu_register : public token
        {
        public:
            virtual ~cpu_register() {}

            virtual bool is_register()
            {
                return true;
            }
        };

        class size_override : public token
        {
        public:
            virtual ~size_override() {}

            virtual bool is_size_override()
            {
                return true;
            }
        };

        class arithmetic_operator : public token
        {
        public:
            virtual ~arithmetic_operator() {}

            virtual bool is_operator()
            {
                return true;
            }
        };

        class segment_override : public token
        {
        public:
            virtual ~segment_override() {}

            virtual bool is_segment_override()
            {
                return true;
            }
        };

        class mnemonic : public token
        {
        public:
            virtual ~mnemonic() {}

            virtual bool is_mnemonic()
            {
                return true;
            }
        };

        class operand : public token
        {
        public:
            virtual ~operand() {}

            virtual bool is_operand()
            {
                return true;
            }
        };

        class address : public token
        {
        public:
            virtual ~address() {}

            virtual bool is_address()
            {
                return true;
            }
        };
    }
}
