# A simple makefile for CSE 100 P3

CC=g++
#CC, default cc
#CXX, default g++
CXXFLAGS=-std=c++11 -g
LDFLAGS=-g
#LDFLAGS, Extra flags to give to compilers when they are supposed to invoke the linker, 
#'ld', such as -L. Libraries (-lfoo) should be added to the LDLIBS variable instead. 

all: compress uncompress

compress: BitInputStream.o BitOutputStream.o HCNode.o HCTree.o

uncompress: BitInputStream.o BitOutputStream.o HCNode.o HCTree.o

HCTree.o: BitInputStream.hpp BitOutputStream.hpp HCNode.hpp HCTree.hpp

HCNode.o: HCNode.hpp

BitOutputStream.o: BitOutputStream.hpp

BitInputStream.o: BitInputStream.hpp


#HCTree.o: HCTree.hpp
#HCTree.o HCNode.o: HCNode.hpp
#HCTree.o BitInputStream.o: BitInputStream.hpp
#HCTree.o BitOutputStream.o: BitOutputStream.hpp
#RM, command to remove a file; default 'rm -f'

.PHONY: clean
clean:
	rm -f compress uncompress *.o core* *~

