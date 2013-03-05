CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -Wno-unused-parameter -I . -Wno-unused-variable
LDFLAGS=-lc++ -lboost_system -lboost_program_options -lboost_filesystem
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

clean: clean-test
	@rm -rfv *.o
	@rm -rfv */*.o

test: $(EXECUTABLE) clean-test $(TESTS) $(TESTRESULTS)

clean-test:
	@rm -rfv tests/*.bin

%.bin: %.asm $(EXECUTABLE)
	@./rasm $< -o $@
	@yasm $< -o $@.ref
	@cmp -s $< $@.ref; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo $<": SAME"; \
	else \
		echo $<": NOT SAME"; \
	fi
