CC=gcc
CFLAGS= -Wall -Werror
LDFLAGS=
EXEC=cracker

all: $(EXEC)

cracker: stack.o main.o reverse.o sha256.o
	$(CC) -pthread -o $@ $^ $(LDFLAGS)

main.o : reverse.h stack.h
stack.o : main.h

%.o: %.c
	$(CC) -pthread -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
