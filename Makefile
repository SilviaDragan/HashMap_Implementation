CC = gcc
CFLAGS = -Wall -g

build: main.o hashmap.o
	$(CC) main.o hashmap.o -o so-cpp

main.o: main.c utils.h
	$(CC) $(CFLAGS) -c main.c

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

run:
	./so-cpp

.PHONY: clean

clean:
	rm -rf so-cpp *.o