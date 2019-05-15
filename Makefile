all: shape_area

shape_area: main.o function.o
	gcc -m32 -o shape_area main.o function.o

main.o: main.c function.h
	gcc -c -m32 -o main.o main.c

function.o: function.asm
	nasm -f elf32 -o function.o function.asm

clean:
	rm -r function.o main.o   