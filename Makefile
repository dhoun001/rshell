CC = g++
CC_FLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir bin
	$(CC) $(CC_FLAGS) src/main.cpp -o bin/rshell.o

rshell:
	mkdir bin
	$(CC) $(CC_FLAGS) src/main.cpp -o bin/rshell.o

clean:
	rm -r bin
