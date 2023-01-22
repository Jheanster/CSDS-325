#Jhean C Toledo
# jct95
# Makefile
# 10/3/22
# This file creates an executable for project 2


CC=gcc
CXX=g++
LD=gcc
CFLAGS=-Wall -Werror -g
LDFLAGS=$(CFLAGS)

TARGETS=proj2

all: $(TARGETS)

sample: proj2.o
	$(CC) $(CFLAGS) -o $@ $< 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cc
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)

