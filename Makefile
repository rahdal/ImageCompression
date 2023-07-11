# Makefile

# Compiler
CXX ?= g++

# Compiler flags
CXXFLAGS ?= --std=c++11 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

main.exe: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out *.in