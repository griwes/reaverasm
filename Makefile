CC=clang++
LD=clang++
CFLAGS=-c -Os -Wall -Wextra -pedantic -Werror -std=c++11 -stdlib=libc++ -Wno-unused-parameter -I .
LDFLAGS=-lc++ -lboost_system -lboost_program_options
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rasm

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rfv *.o
	@rm -rfv */*.o
