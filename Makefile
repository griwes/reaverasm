CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -I . -g -MD -pthread -Wno-unused-private-field
LDFLAGS=-stdlib=libc++ -lc++abi -lc++ -lboost_system -lboost_program_options -lboost_filesystem -pthread
SOURCES=$(shell find . -type f -name "*.cpp" ! -path "*old*")
OBJECTS=$(SOURCES:.cpp=.o)
TESTS=$(shell find . -name "*.asm" ! -name "*.elf.asm")
ELFTESTS=$(shell find . -name "*.elf.asm")
TESTRESULTS=$(TESTS:.asm=.bin) $(ELFTESTS:.elf.asm=)
EXECUTABLE=rasm

all: $(SOURCES) $(EXECUTABLE)

install:
	@sudo mkdir -p /usr/local/include/reaver/assembler
	@sudo cp frontend/frontend.h /usr/local/include/reaver/assembler
	@sudo cp generator/generator.h /usr/local/include/reaver/assembler
	@sudo cp output/output.h /usr/local/include/reaver/assembler
	@sudo cp parser/parser.h /usr/local/include/reaver/assembler
	@sudo cp preprocessor/preprocessor.h /usr/local/include/reaver/assembler

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) -lreaver -pthread

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean: clean-test
	@find . -name "*.o" -delete
	@find . -name "*.d" -delete

test: $(EXECUTABLE) $(TESTS) $(ELFTESTS) $(TESTRESULTS)

clean-test:
	@rm -rfv tests/*.bin
	@rm -rfv tests/*.elf

%.bin: %.asm $(EXECUTABLE) clean-test
	./rasm $< -o $@ -s

%: %.elf.asm $(EXECUTABLE) clean-test
	./rasm $< -o $@.elf -f elf64 -s
	ld $@.elf -lc -o $@ -s -dynamic-linker /lib64/ld-linux-x86-64.so.2
	./$@

-include $(SOURCES:.cpp=.d)
