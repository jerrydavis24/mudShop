# Set compiler variables
CC = gcc
CXX = g++  # Use g++ for C++ files

# Compiler flags
BASE_CFLAGS = -Wall -Werror -g -fPIE -std=c++11  # Add -std=c++11 for C++11 support
CFLAGS = $(BASE_CFLAGS)
CXXFLAGS = $(CFLAGS)  # Use the same flags for C++ files

##########################################################
# Object files variable that contains a list of the object files that will be built
OBJS = main.o data.o operations.o shop.o  # CODE HERE: list all .o files to be created included main.o
##########################################################

# Default target all runs the compilation for main and test
all: main.bin

# Main program target
main.bin: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main.bin $(OBJS)  # Remove -pie here

# Generic rule for building C++ object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Dependencies
##########################################################
# CODE HERE: insert all dependencies from .o to .h files
##########################################################
main.o: main.cpp data.h operations.h shop.h
data.o: data.h
operations.o: operations.h 
shop.o: shop.h

# Clean target
clean:
	rm -f main.bin test.bin *.o

# .PHONY will cause the Makefile to consider the targets "all" and "clean" as always out-of-date
.PHONY: all clean
