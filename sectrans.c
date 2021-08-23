/*
	This progrem is exacuting the second transformation on the assembler file .
	The program returns a struct of 3 linked lists ready to be print into files: objects, entry and extern.
*/

#include "assembler.h"

#define TWOTWENTYFIVE 33554432
#define OPCODE 26
#define BNECODE 15
#define BGTCODE 18
#define JMPCODE 30
#define LACODE 31
#define CALLCODE 32 

char firstchar (char *line);
char *firstword (char *line);
char *nextword (char *line);
int istag (char *p);
int finddata(char *ptr);
int isexen (char *p);
Symbol_node *findsymbol(Symbol_node *head, char *symbol);
void addentry(Symbol_node *head, char *line);
int calcsymbol(Symbol_node *head, char *line, int bcode, int address, int type);
int extpush(Extern_node *head, int value, char *symbol);
void mergelist (Bcode_node *head, Bcode_node *tail);
int stablepush(char *symbol, int value, int att, Symbol_node *head);
int findtype(char *ptr);
int isemptybnode(Bcode_node *node);
int isemptysymnode(Symbol_node *node);
Firsttrans *firsttrans (char *ifp);

Sectrans *sectrans (FILE  *ifp, Firsttrans *firsttrans)
{
	int i = 0, temp = 0, k = 0;
	Bcode_node *IC = firsttrans -> IC;
	Bcode_node *DC = firsttrans -> DC;
	Extern_node *EX_head = (Extern_node *)malloc(sizeof(Extern_node));
	Sectrans *toprint = (Sectrans *)malloc(sizeof(Sectrans));
	char *line = (char *)calloc(MAXLINES, sizeof(char));
	
	while ((fgets (line, MAXLINES, ifp)) != NULL){
		if (firstchar(line) == '\0' || firstchar(line) == ';' || istag(firstword(line)));/*empty, comment or tag*/
		else{
			if (istag(firstword(line))){/*Tag*/
				while(*line != ':'){
					line++;
					k++;
				}
				line++;/*point one step after ":"*/
				k++;
			}

			while(!isgraph(*line))/*point to next word*/
				line++;

			if (finddata(firstword(line)) != ERROR){		
				if(DC -> next != NULL)							
					DC = DC -> next;
			}
			else if(isexen(line) == ENTRY){
				addentry(firsttrans -> Symbol, line);	
			}

			else{
				if (findtype(firstword(line)) == ITYPE){
					i = IC -> b.bcode;
					temp = i>>OPCODE;/*temp is the opcode*/ 
					if (temp >= BNECODE && temp <= BGTCODE)
						IC -> b.bcode = calcsymbol(firsttrans -> Symbol, line-k, i, IC -> b.address, 0);
									
				}
				else if (findtype(firstword(line)) == JTYPE){
					i = IC -> b.bcode;
					temp = i>>OPCODE;/*temp is the opcode*/ 
					if (temp >= LACODE && temp <= CALLCODE )
						IC -> b.bcode = calcsymbol(firsttrans -> Symbol, line-k, i, IC -> b.address, 1);
					else if (temp == JMPCODE && ((i&TWOTWENTYFIVE) == 0))/*jmp with reg zero*/
						IC -> b.bcode = calcsymbol(firsttrans -> Symbol, line-k, i, IC -> b.address, 1);
					if(i == IC -> b.bcode && findsymbol(firsttrans -> Symbol, nextword(line)) != NULL)/*indicats on an external tag*/
						extpush(EX_head, IC -> b.address, findsymbol(firsttrans -> Symbol, nextword(line)) -> symbol);
				}


				if(IC -> next != NULL)
					IC = IC -> next;							
			}
		}	
		
		temp = 0;
		k = 0;


	}

	mergelist(firsttrans -> IC, firsttrans -> DC);
	toprint -> ob = firsttrans -> IC;
	toprint -> ent = firsttrans -> Symbol;
	toprint -> ext = EX_head;

	return toprint;
}



