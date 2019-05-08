CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -Werror
LDFLAGS=
EXEC=cracker

all: $(EXEC)

cracker: stack.o main.o reverse.o sha256.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o : reverse.h stack.h
stack.o : main.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
