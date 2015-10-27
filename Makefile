all:
	mkdir -p bin
	gcc -o bin/test main.c draw.c -lncurses
