CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++1y -stdlib=libc++ -g -MD -pthread -fPIC
LDFLAGS=-stdlib=libc++ -lc++abi -lc++ -lboost_system -lboost_program_options -lboost_filesystem -pthread
SOFLAGS=-stdlib=libc++ -shared -pthread
SOURCES=$(shell find . -type f -name "*.cpp" ! -path "*-old*" ! -path "./main.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
TESTS=$(shell find . -name "*.asm" ! -name "*.elf.asm")
ELFTESTS=$(shell find . -name "*.elf.asm")
TESTRESULTS=$(TESTS:.asm=.bin) $(ELFTESTS:.elf.asm=)
LIBRARY=libreaverasm.so
EXECUTABLE=rasm

all: $(SOURCES) $(LIBRARY) $(EXECUTABLE)

library: $(LIBRARY)

library-install: $(LIBRARY)
	@sudo mkdir -p /usr/local/include/reaver/assembler
	@find . -name "*.h" ! -path "*-old" ! -name "assembler.h" | sudo cpio -pdm /usr/local/include/reaver/assembler 2> /dev/null
	@sudo cp assembler.h /usr/local/include/reaver
	@sudo cp libreaverasm.so /usr/local/lib/libreaverasm.so.1
	@sudo ln -sfn /usr/local/lib/libreaverasm.so.1 /usr/local/lib/libreaverasm.so

$(EXECUTABLE): library-install main.o
	$(LD) $(LDFLAGS) -o $@ main.o -lreaver -pthread -lreaverasm

$(LIBRARY): $(OBJECTS)
	$(LD) $(SOFLAGS) -o $@ $(OBJECTS) -lreaver

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean: clean-test
	@find . -name "*.o" -delete
	@find . -name "*.d" -delete
	@find . -name "*.so" -delete
	@rm -rf rasm

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
-include main.d
