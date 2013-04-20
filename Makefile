CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -Wno-unused-parameter -I . -Wno-unused-variable \
	-Wno-unused-private-field
LDFLAGS=-stdlib=libc++ -lc++abi -lc++ -lboost_system -lboost_program_options -lboost_filesystem
DIRECTORIES=$(shell find -type d -not -name "*-old*")
SOURCES=$(shell find $(DIRECTORIES) -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
TESTS=$(shell find . -name "*.asm")
TESTRESULTS=$(TESTS:.asm=.bin)
EXECUTABLE=rasm

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) -lreaver

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean: clean-test
	@rm -rfv *.o
	@rm -rfv */*.o

test: $(EXECUTABLE) $(TESTS) $(TESTRESULTS)

clean-test:
	@rm -rfv tests/*.bin

%.bin: %.asm $(EXECUTABLE) clean-test
	@./rasm $< -o $@
	@yasm $< -o $@.ref
	@cmp -s $@ $@.ref; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo $<": SAME"; \
	else \
		echo $<": NOT SAME"; \
	fi
