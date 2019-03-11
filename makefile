assembler.o: main.c
	gcc structures.c utils.c first_scan.c second_scan.c main.c -Wall -ansi -pedantic -o assmebler