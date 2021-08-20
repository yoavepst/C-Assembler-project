#include "assembler.h"

#define 2_^_25 33554432
#define OPCODE 26
#define BNECODE 15
#define BGTCODE 18
#define JMPCODE 30
#define LACODE 31
#define CALLCODE 32 

Sectrans sectrans (FILE *ifp, Firsttrans *firsttrans)
{
	int i = 0, temp = 0, k = 0;
	Bcode_node *IC = Firsttrans -> IC;
	Bcode_node *DC = Firsttrans -> DC;
	Extern_node *EX_head = (Extern_node *)malloc(sizeof(Extern_node));
	Sectrans toprint = (Sectrans *)malloc(sizeof(Sectrans));
	
	while (fgets(line, MAXLINES, ifp) != NULL){
		if (firstchar(line) == '\0' || firstchar(line) == ":" || istag(firstword(line)));/*empty, comment or tag*/
		
		else if (istag(firstword(line))){/*Tag*/
			tag = 1;
			while(*line != ":"){
				line++;
				k++;
			}
			line++;/*point one step after ":"*/
			k++;
		}

		while(!isgraph(*line))/*point to next word*/
			line++;

		if (finddata(line) != ERROR && !isexen){		
			if(DC -> next != NULL)							
				DC = DC -> next;
		}
		else if(isexen(line) == ENTRY){
			addentry(firsttrans -> Symbol, line);	
		}

		else{
			if (finddata(line) == ITYPE){
				i = IC -> b.bcode;
				temp = i>>OPCODE;/*temp is the opcode*/ 
				if (temp >= BNECODE && temp <= BGTCODE)
					IC -> b.bcode = calcsymbol(firsttrans -> Symbol, line-k, i, IC -> b.address, 0);
									
			}
			else if (finddata(line) == JTYPE){
				i = IC -> b.bcode;
				temp = i>>OPCODE;/*temp is the opcode*/ 
				if (temp >= LACODE && temp <= CALLCODE )
					IC -> b.bcode = calcsymbol(firsttrans -> Symbol, line-k, i, IC -> b.address, 1);
				else if (temp == JMPCODE && (i&(2_^_25) == 0))/*jmp with reg zero*/
					IC -> b.bcode = calcsymbol(firsttrans -> Symbol, line-k, i, IC -> b.address, 1);
				if(i == IC -> b.bcode)/*indicats on an external tag*/
					extpush(EX_head, IC -> b.address, findsymbol(firsttrans -> Symbol, nextword(line)));
			}


			if(IC -> next != NULL)
				IC = IC -> next;							
		}
			
		
		i = 0;
		temp = 0;
		k = 0;


	}
	intorder(firsttrans -> DC)/*NTBP*/;
	mergelist(firsttrans -> IC, firsttrans -> DC);
	toprint -> ob = firsttrans -> IC;
	toprint -> ent = expush(firsttrans -> Symbol);
	toprint -> ext = EX_head;

	return toprint;
}
