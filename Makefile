CC=gcc
CFLAGS= -Wall -Werror
LDFLAGS=
EXEC=cracker

basic : $(EXEC)

all : $(EXEC)
	./cracker -t 3 test/1.bin test/2.bin
	./cracker -t 3 -o output.txt test/2.bin

cracker: src/stack.o src/main.o src/reverse.o src/sha256.o
	$(CC) -pthread -o $@ $^ $(LDFLAGS)

main.o : src/reverse.h src/stack.h
stack.o :src/ main.h
reverse.o : src/sha256.h

%.o: %.c
	$(CC) -pthread -o $@ -c $< $(CFLAGS)

erase:
	rm -rf src/main.o
	rm -rf src/reverse.o
	rm -rf src/sha256.o
	rm -rf src/stack.o
	rm -rf output.txt

clean: erase
	rm -rf $(EXEC)
