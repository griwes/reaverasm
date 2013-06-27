CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -I . -g -MD
LDFLAGS=-stdlib=libc++ -lc++abi -lc++ -lboost_system -lboost_program_options -lboost_filesystem
SOURCES=$(shell find . -type f -name "*.cpp" ! -path "*-old*")
OBJECTS=$(SOURCES:.cpp=.o)
TESTS=$(shell find . -name "*.asm" ! -name "*.elf.asm")
ELFTESTS=$(shell find . -name "*.elf.asm")
TESTRESULTS=$(TESTS:.asm=.bin) $(ELFTESTS:.elf.asm=)
EXECUTABLE=rasm

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) -lreaver -pthread

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean: clean-test
	@rm -rfv *.o
	@rm -rfv */*.o
	@rm -rfv *.d
	@rm -rfv */*.d

test: $(EXECUTABLE) $(TESTS) $(ELFTESTS) $(TESTRESULTS)

clean-test:
	@rm -rfv tests/*.bin
	@rm -rfv tests/*.elf

%.bin: %.asm $(EXECUTABLE) clean-test
#	@yasm $< -o $@.ref
	@./rasm $< -o $@
#	@cmp -s $@ $@.ref; \
#	RETVAL=$$?; \
#	if [ $$RETVAL -eq 0 ]; then \
#		echo $<": SAME"; \
#	else \
#		echo $<": NOT SAME"; \
#	fi

%: %.elf.asm $(EXECUTABLE) clean-test
#	@yasm $< -o $@.elf.ref -f elf64
	./rasm $< -o $@.elf -f elf64
	ld $@.elf -lc -o $@ -s -dynamic-linker /lib64/ld-linux-x86-64.so.2
	./$@
#	@cmp -s $@ $@.ref; \
#	RETVAL=$$?; \
#	if [ $$RETVAL -eq 0 ]; then \
#		echo $<": SAME"; \
#	else \
#		echo $<": NOT SAME"; \
#	fi

-include $(SOURCES:.cpp=.d)
