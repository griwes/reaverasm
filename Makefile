CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -Wno-unused-parameter -I . -Wno-unused-variable \
	-Wno-unused-private-field -g -MD
LDFLAGS=-stdlib=libc++ -lc++abi -lc++ -lboost_system -lboost_program_options -lboost_filesystem
SOURCES=$(shell find . -type f -name "*.cpp" ! -path "*-old*")
OBJECTS=$(SOURCES:.cpp=.o)
TESTS=$(shell find . -name "*.asm" ! -name "*.elf.asm")
ELFTESTS=$(shell find . -name "*.elf.asm")
TESTRESULTS=$(TESTS:.asm=.bin) $(ELFTESTS:.elf.asm=.elf)
EXECUTABLE=rasm

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) -lreaver

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean: clean-test
	@rm -rfv *.o
	@rm -rfv */*.o

test: $(EXECUTABLE) $(TESTS) $(ELFTESTS) $(TESTRESULTS)

clean-test:
	@rm -rfv tests/*.bin
	@rm -rfv tests/*.elf

%.bin: %.asm $(EXECUTABLE) clean-test
	@yasm $< -o $@.ref
	@./rasm $< -o $@
	@cmp -s $@ $@.ref; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo $<": SAME"; \
	else \
		echo $<": NOT SAME"; \
	fi

%.elf: %.elf.asm $(EXECUTABLE) clean-test
	@yasm $< -o $@.ref -f elf64
	@./rasm $< -o $@ -f elf64
	@cmp -s $@ $@.ref; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo $<": SAME"; \
	else \
		echo $<": NOT SAME"; \
	fi

-include $(SOURCES:.cpp=.d)
