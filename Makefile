CC=gcc
CFLAGS= -Wall -Werror
LDFLAGS=
EXEC=cracker

all: $(EXEC)

cracker: stack.o main.o reverse.o sha256.o
	$(CC) -pthread -o $@ $^ $(LDFLAGS)

main.o : reverse.h stack.h
stack.o : main.h
reverse.o : sha256.h

%.o: %.c
	$(CC) -pthread -o $@ -c $< $(CFLAGS)

erase:
	rm -rf *.o

clean: erase
	rm -rf $(EXEC)
