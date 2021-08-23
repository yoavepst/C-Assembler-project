/*
	This program translate an assembler file into a machine code output file. It mimics an assambler work for a 32-bit CPU.
	This project is an assignment in a C programming course at the Open Unvieristy of Israel.
	The project auther - Yoav Epstein.
	Source structure:
	1. main - main function 
	2. firsttrans - contains the first transition function
	3. sectrans - contains the second transition function
	4. Bcode_node - contains the functions related to the binary code node list 
	5. functions - contains a generic functions of the proggram 
	6. instrucations - contains the functions related to instructions processing 
	7. permtables - contains the functions initiating the permanent commands table
	8. symtable - contains the functions initiating the symbol table
	9. assembler.h - contains liberies and structs defenitions
*/

#include "assembler.h"

#define INTSIZE 32
#define CHARSIZE 8
#define ENT1 10
#define ENT2 12
#define ENT3 13

Firsttrans *firsttrans (FILE *ifp);
Sectrans *sectrans (FILE * ifp, Firsttrans *firsttrans);
int fassemblerprint(Sectrans *toprint, char *filename);
int getaddress (Bcode_node *bnode);
int getbcode (Bcode_node *bnode);
char *getexsymbol (Extern_node *ext);
int getvalue (Extern_node *ext);
int getatt (Symbol_node *symbol);
char *getsymbol (Symbol_node *symbol);

int main (int argc, char *argv[])
{
	
	FILE *ifp = (FILE *)malloc(sizeof(FILE));
	Firsttrans *trans;
	char *filename = "";

	if (argc == 1)
		return 1;

	while (--argc > 0)
	{
		if((ifp = fopen(*++argv, "r")) == NULL)
			printf("Cannot open file %s, continuing with the next file\n", *argv);

		else{
			trans = firsttrans(ifp);
			fassemblerprint(sectrans(ifp, trans), strcpy(filename, *argv));
			fclose (ifp);
			free(trans);
		}
	}

	return 0;

}

int fassemblerprint(Sectrans *toprint, char *filename)
{
	FILE *ob, *ext, *ent;
	unsigned int i, k;
	unsigned char temp;
	Bcode_node *obptr = toprint -> ob;
	Extern_node *extptr = toprint -> ext; 
	Symbol_node *entptr = toprint -> ent;

	/*object file*/
	ob = fopen(strcat(filename, ".ob"), "a+");
	while(obptr != NULL){
		fputc(0, ob);/*NTBP*/
		fprintf(ob, "%d", getaddress(obptr));
		fputc('\t', ob);
		k = getbcode(obptr);
		for(i = INTSIZE/CHARSIZE ; i > 0; i--){
			temp = (unsigned char)k;
			fprintf(ob, "%02x", temp);
			fputc('\t', ob);
			k = (k>>CHARSIZE);
		}
		fputc('\n', ob);
		obptr = obptr-> next; 
	}
	
	fclose(ob);

	/*extern file*/
	if (extptr != NULL){/*creats the file onlt if there are externals*/
		ext = fopen(strcat(filename, ".ext"), "a+");
		while(extptr != NULL){
			fputs(getexsymbol(extptr), ext);
			fputc('\t', ext);
			fputc(0, ext);
			fprintf(ext, "%d", getvalue(extptr));
			fputc('\n', ext);
			extptr = extptr-> next; 	
		}	
			fclose(ext);
	}

	/*entry file*/
	if (entptr != NULL){/*creats the file onlt if there are entries*/
		ent = fopen(strcat(filename, ".ent"), "a+");
		while(entptr != NULL){
			if(getatt(entptr) == ENT1 || getatt(entptr) == ENT2 || getatt(entptr) == ENT3){
				fputs(getsymbol(entptr), ent);
				fputc('\t', ent);
				fputc(0, ent);
				fprintf(ent, "%d", getvalue(extptr));
				fputc('\n', ent);
			}
			entptr = entptr-> next; 	
		}	
			fclose(ent);
	}
	return 1;
}
