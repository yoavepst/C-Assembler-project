sectrans: Bcode_node.c functions.c instrucations.c permtables.c symtable.c firsttrans.c sectrans.c main.c assembler.h
	gcc -g -ansi -Wall -pedantic Bcode_node.c functions.c instrucations.c permtables.c symtable.c firsttrans.c sectrans.c main.c -o assambler
