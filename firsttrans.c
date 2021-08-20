#include "assembler.h"

char firstchar (char *line);
char *firstword (char *line);
int finddata(char *ptr);
int stablepush(char *symbol, int value, int att, Symbol_node *head);
int datalength (INS *in);
int mempush(int binarycode, Bcode_node *head, int value);
int datatoBcode (INS *in, int length);
INS *datatostruct(char *line, int datatype);
int isexen (char *p);
int findtype(char *ptr);
unsigned int codetoBcode(char *line, int typenum);
int istag (char *p);
int isemptybnode(Bcode_node *node);
int isemptysymnode(Symbol_node *node);

Firsttrans *firsttrans (FILE *ifp)
{
	static int IC = 100; /*initializing the first adderss of the assembler"*/
	static int DC = 0; /*initializing the first adderss of the assembler"*/
	static int LC = 1; /*pointing on the line number in the file*/
	char *line = (char *)calloc(MAXLINES, sizeof(char));
	char *temp = (char *)calloc(MAXTAG, sizeof(char));
	int i = 0, tag = 0, k = 0, data = ERROR;
	enum attributes att;
	
	/*struct pointers*/
	INS *in;
	Bcode_node *IC_head;
	Bcode_node *DC_head;
	Bcode_node *currentdc;
	Symbol_node *Symbol_head;
	Symbol_node *currentsymbol;
	
	Firsttrans *toprint;

	/*initializing a linked lists and the struct that gethers them*/
	toprint = (Firsttrans *)calloc(2, sizeof(Firsttrans));
	IC_head = (Bcode_node *)malloc(sizeof(Bcode_node));
	DC_head = (Bcode_node *)malloc(sizeof(Bcode_node));
	Symbol_head = (Symbol_node *)malloc(sizeof(Symbol_node));
	line = fgets(line, MAXLINES, ifp);
	ftell(ifp);
	i = fgetc(ifp);
	while((fgets(line, MAXLINES, ifp)) != NULL){
		if (firstchar(line) == '\0' || firstchar(line) == ';')/*empty or comment*/
			break;
		temp = firstword(line);
		if (istag(temp)){/*Tag*/
			tag = 1;
			while(*line != ':'){
				line++;
			}
			line++;/*point one step after ":"*/
			strncpy(temp,temp,strlen(temp)-1);
		}
		
		while(!isgraph(*line))/*point to next word*/
			line++;

		if (finddata(line) != ERROR){/*.dh, .dw, .db, .asciz*/
			if(tag)
				stablepush(temp, DC, att = DATA, Symbol_head);
			data = finddata(line);
			in = datatostruct(line, data);
			k = datalength(in);
			for(i = 0; i < k; i++){
				mempush(datatoBcode(in, data), DC_head, DC);
				DC += (data == 3 ? 0 : (data) + 1);
			}
		}

		else if (isexen(line) != ERROR)/*.extern, .entry*/
		{
			if (!strcmp(firstword(line), ".external"))
				stablepush(temp, DC, att = EXTERNAL, Symbol_head);
		}

		else if ((k = findtype(firstword(line))) != ERROR){			
			if(tag)
				stablepush(temp, IC, att = CODE, Symbol_head);
			mempush(codetoBcode(line, k), IC_head, IC);
			IC += 4;
			
		}
		
		tag = 0;
		i = 0;
		LC++;
		
	}

	currentdc = DC_head;
	currentsymbol = Symbol_head;

	while(!isemptybnode(currentdc)){
		currentdc -> b.address += IC;
		currentdc = currentdc -> next;  		
	}

	while(!isemptysymnode(currentsymbol)){
		if(currentsymbol -> att == DATA)
			currentsymbol -> value += IC;
		currentsymbol = currentsymbol -> next;  		
	}
		
	
	toprint -> IC = IC_head;
	toprint -> DC = DC_head;
	toprint -> Symbol = Symbol_head;

	return toprint;
}
