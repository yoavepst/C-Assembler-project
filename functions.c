/*
	This progrem is a collection of generic functions of the assembler.
*/

#include "assembler.h"

static const char *Rname[] = {"\0", "add", "sub", "and", "or", "nor", "move", "mvhi", "mvlo"};
static const char *Iname[] = {"\0", "addi", "subi", "andi", "ori", "nori", "bne", "beq", "blt", "bgt", "lb", "sb", "lw", "sw", "lh", "sh"};
static const char *Jname[] = {"\0", "jmp", "la", "call", "stop"};
static const char  *attributesname[] = {".extern", ".entry"}; 

/*return the first char of an assembler line*/
char firstchar (char *line)
{
	int i = 0;
	
	while(i < strlen(line)){
		if(isspace(line[i]))
			i++;
		else 	
			return line[i];
	}
		
	return '\0';
}

/*copy the first word to s, and return it*/
char *firstword (char line[])
{
	int i = 0, j = 0;
	char *s, *ptr;
	
	s = (char *)calloc(MAXTAG ,sizeof(char));
	
	while(i < strlen(line)){
		if(isspace(line[i]))
			i++;
		else{ 	
			j = i;
			ptr = &line[i];			
			while(j-i < MAXTAG){			
				if(!isspace(line[j]) && line[j] != '\0')
					j++;
				else{
					return strncpy(s, ptr, j-i);
				} 	
							
			}
		}
	}
		
	return 0;
}

/*copy the next word to s, and return it*/
char *nextword (char *line)
{
	int i = 0, j = 0;
	char *s, *ptr;
	
	s = (char *)malloc(sizeof(char));
	
	/*point on the next space or ','*/
	while(i < strlen(line)){
		if(!isspace(line[i]))
			i++;
		else 
			break;
	}

	/*copy the next word to s, and return it*/
	while(i < strlen(line)){
		if(isspace(line[i]))
			i++;
		else{ 	
			j = i;
			ptr = &line[i];			
			while(j-i < MAXTAG){			
				if(!isspace(line[j]) && line[j] != '\0')
					j++;
				else{
					return strncat(s, ptr, j-i);
				} 	
							
			}
		}
	}
		
	return NULL;
}

/*Boolean yes or no for the question whether a string is any of the commands (the upcoming 3)*/
int isrtype (char *p)
{
	int i = 0;	
	while(i <= RTYPENUM){
		if (!strcmp(p, Rname[i]))
			return i;
		else
			i++;
	}
	
	return 0;
}

int isitype (char *p)
{
	int i = 0;	
	while(i <= ITYPENUM){
		if (!strcmp(p, Iname[i]))
			return i;
		else
			i++;
	}
	
	return 0;
}

int isjtype (char *p)
{
	int i = 0;	
	while(i <= JTYPENUM){
		if (!strcmp(p, Jname[i]))
			return i;
		else
			i++;
	}
	
	return 0;
}
/*Boolean yes or no for the question whether a string is a forbidden tag*/
int forbiddentag (char *p)
{
	char *c = (char *)calloc(MAXTAG, sizeof(char));

	if (strlen(p)-1 > MAXTAG)
		return 1;
	else{
		strncpy(c, p, strlen(p)-1); /*Copys the tag without ':'*/
		if (isrtype(c) || isitype(c) || isjtype(c))
			return 1;
	}
	return 0;
		
}
/*Boolean yes or no for the question whether a string contains a tag*/
int istag (char *p)
{
	int i = strlen(p)-1;
	if (isalpha(*p) && p[i] == ':')
		if(!forbiddentag(p))		
			return strlen(p)-1;
	return 0;
		
}
/*Boolean yes or no for the question whether a string is any of the commands (the upcoming 3)*/
int isexen (char *p)
{
	if(*p == '.'){
		if (!strncmp(p, attributesname[EXTERNAL], strlen(attributesname[EXTERNAL])))
			return EXTERNAL;  
		else if (!strncmp(p,attributesname[ENTRY], strlen(attributesname[ENTRY])))
			return ENTRY;  
	}
	return ERROR;	
			
}
/*pushing a node to the and of extern variables list*/
int extpush(Extern_node *head, int value, char *symbol)
{
	Extern_node *current;
	current = head;

	while (current -> next != NULL){
			current = current -> next;
	}

	current -> next = (Extern_node *)malloc(sizeof(Extern_node));
	current = current -> next;  		
	current -> symbol = symbol;
	current -> value =  value;
	
	return 1;
}

/*get the extern node's symbol*/
char *getexsymbol (Extern_node *ext)
{
if (ext -> symbol != '\0')
		return ext -> symbol;
	return '\0';
}

/*get the extern node's value*/
int getvalue (Extern_node *ext)
{
if (ext -> value != ERROR)
		return ext -> value;
	return ERROR;
}
