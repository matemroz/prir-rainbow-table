CC=cc
RM=rm
CCFLAGS=-c -Wall --pedantic
CFLAGS=-c
LIBS=-lcrypt
OBJS=main.o file_reader.o rainbow.o rainbow_cracker.o sorter.o

all: rainbowtab

rainbowtab: $(OBJS)
	$(CC) -o rainbowtab $(OBJS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

file_manager.o: file_manager.c file_manager.h
	$(CC) $(CFLAGS) file_manager.c

rainbow.o: rainbow.c rainbow.h
	$(CC) $(CFLAGS) rainbow.c $(LIBS)

rainbow_cracker.o: rainbow_cracker.c rainbow_cracker.h
	$(CC) $(CFLAGS) rainbow_cracker.c

file_manager.o: file_manager.c file_manager.h
	$(CC) $(CFLAGS) file_manager.c

clean:
	$(RM) *.o rainbowtab

.PHONY: all clean test

test:
	./rainbowtab
