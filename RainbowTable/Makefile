CC=mpicc
RM=rm
CCFLAGS=-c -Wall --pedantic
CFLAGS=-c
LIBS=-lcrypt
OBJS=main.o file_reader.o rainbow.o rainbow_cracker.o sorter.o formatter.o file_writer.o

all: rainbowtab

rainbowtab: $(OBJS)
	$(CC) -o rainbowtab $(OBJS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

file_reader.o: file_reader.c
	$(CC) $(CFLAGS) file_reader.c

rainbow.o: rainbow.c rainbow.h
	$(CC) $(CFLAGS) rainbow.c $(LIBS)

rainbow_cracker.o: rainbow_cracker.c rainbow_cracker.h
	$(CC) $(CFLAGS) rainbow_cracker.c

formatter.o: formatter.c
	$(CC) $(CFLAGS) formatter.c

file_writer.o: file_writer.c
	$(CC) $(CFLAGS) file_writer.c

clean:
	$(RM) *.o rainbowtab

.PHONY: all clean test

test:
	./rainbowtab
