#include "assembler.h"

#define INTSIZE 32
#define CHARSIZE 8
#define ENT1 10
#define ENT2 12
#define ENT3 13
/*
	this program translate an assembler file into a machine code output file.
	An assembler line in the files has a same structure: possiable tag, commend/instruction, parametrs
*/

int main (int argc, char *argv[])
{
	
	FILE *ifp;
	Bcode_node *trans;
	char *filename;

	if (argc == 1)
		return 1;

	while (--argc > 0)
	{
		if(ifp = fopen(*++argv, "r") == NULL)
			printf("Cannot open file %s, continuing with the next file\n", *argv);

		else{
			trans = &firsttrans(ifp);
			fassemblerprint(secondtrans(ifp, trans), strcpy(filename, argv));
			fclose (ifp);
			free(trans);
		}
	}

		


}

int fassemblerprint(Sectrans toprint, char *filename)
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
		putw(getaddress(obptr), ob);
		fputc('\t', ob);
		k = getbcode(obptr);
		for(i = INTSIZE/CHARSIZE ; i > 0; i--){
			temp = (unsigned char)k;
			fputs(ctox(temp)/*NTBP*/, ob);
			fputc('\t', ob);
			k>>CHARSIZE;
		}
		fputc('\n', ob);
		obptr = obptr-> next; 
	}
	
	fclose(ob);

	/*extern file*/
	if (extptr != NULL){/*creats the file onlt if there are externals*/
		ext = fopen(strcat(filename, ".ext"), "a+");
		while(extptr != NULL){
			fputs(getsymbol(extptr), ext);
			fputc('\t', ext);
			fputc(0, ext);
			putw(getvalue(extptr), ext);
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
				putw(getvalue(entptr), ent);
				fputc('\n', ent);
			}
			entptr = entptr-> next; 	
		}	
			fclose(ent);
	}
	return 1;
}
