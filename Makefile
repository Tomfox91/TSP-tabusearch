MAKEFLAGS += --no-builtin-rules
.SUFFIXES:
.PHONY := clean

CC := g++
ADDFLAGS :=
OPTFLAGS := -O3 -flto 
CCFLAGS := -Wall -Wextra -std=c++0x -iquote . $(OPTFLAGS) $(ADDFLAGS)

CPP_FILES := $(shell find . -name '*.cpp')
H_FILES := $(shell find . -name '*.h')
O_FILES := $(CPP_FILES:.cpp=.o)

main: $(O_FILES)
	$(CC) $(CCFLAGS) $^ -o main

%.o: %.cpp $(H_FILES)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	find . -name '*.o' -exec rm -vf {} +