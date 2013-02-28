CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -Wno-unused-parameter -I . -Wno-unused-variable
LDFLAGS=-lc++ -lboost_system -lboost_program_options
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
TESTS=$(shell find . -name "*.asm")
TESTRESULTS=$(TESTS:.asm=.bin)
EXECUTABLE=rasm

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rfv *.o
	@rm -rfv */*.o
	@rm -rfv tests/*.bin

test: clean $(TESTS) $(TESTRESULTS)

%.bin: %.asm
	@yasm $< -o $@.ref
	@./rasm $< -o $@
	@cmp -s $< $@.ref; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo $<": SAME"; \
	else \
		echo $<": NOT SAME"; \
	fi
