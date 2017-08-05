all:
	mkdir -p bin
	gcc -o bin/tetramaster main.c draw.c -lncurses
