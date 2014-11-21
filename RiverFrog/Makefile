#
# Makefile for OS_homework2
#
#
CC=gcc
# if you want to use debugger, add -g to CFLAGS and LDFLAGS
CFLAGS=-c -Wall 
LDFLAGS=
LKFLAGS=-lpthread -lm
SOURCES=frog.c
OBJECTS=$(SOURCES:.c=.o) 
EXECUTABLE=frog
INCLUDES=

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LKFLAGS)

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.out *.exe *.stackdump score $(EXECUTABLE)
