#include "assembler.h"

#define ENT_ADD 10
#define TWOsixteenONE 65535

char *nextword (char *line);

int isemptysymnode(Symbol_node *node)
{
	if(node == NULL || (node -> symbol == NULL && node -> value ==  0 && node -> att == 0))
		return 1;
	return 0;
}

int stablepush(char *symbol, int value, int att, Symbol_node *head)
{
	Symbol_node *current;
	current = head;

	while (current -> next != NULL){
		if(current -> symbol == symbol)
			return ERROR;
		else
			current = current -> next;
	}

	if(isemptysymnode(current)){/*first on the list*/
		current -> symbol = symbol;
		current -> value =  value;
		current -> att =  att;
	}
	else{
		current -> next = (Symbol_node *)malloc(sizeof(Symbol_node));
		current = current -> next;  		
		current -> symbol = symbol;
		current -> value =  value;
		current -> att =  att;
	}

	return 1;
}

Symbol_node *findsymbol(Symbol_node *head, char *symbol)
{
	Symbol_node *ptr = (Symbol_node *)malloc(sizeof(Symbol_node));
	ptr = head;
	for(;ptr != NULL  ;ptr = ptr -> next){
		if(!strcmp(head -> symbol, symbol))
				return ptr;	
	}
	
	return NULL;	

}

void addentry(Symbol_node *head, char *line)
{
	char ptr[MAXTAG];	
	Symbol_node *symbol;

	strcpy(ptr, nextword(line));
	
	symbol = findsymbol(head, ptr);
	symbol -> att =+ ENT_ADD;
}

int calcsymbol(Symbol_node *head, char *line, int bcode, int address, int type)	
{
	int newcode = bcode, temp = 0;
	char *ptr = line;
	Symbol_node *symbol;
	switch(type){
		case 0:
			symbol = findsymbol(head, ptr);
			if(symbol -> value){
				temp = address - symbol -> value;
				newcode |= (temp & (TWOsixteenONE));
			}
			/*elseerror*/	
		break;

		case 1:
			symbol = findsymbol(head, ptr);
			if(symbol -> value){
				temp = address - symbol -> value;
				newcode |= (temp & (TWOsixteenONE));
			}
		break;

	}

	return newcode;
}

