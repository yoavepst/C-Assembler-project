/*
	This progrem is exacuting the assembler's commends table.
	There are 3 types and more sub-types. Every type is initallized in according to it setting.
*/

#include "assembler.h"

#define jopcode 29
#define iopcode 9

int isjtype (char *p);
int isitype (char *p);
int isrtype (char *p);

/*The upcoming 3 functions transform the varius commend structs into a binary code*/
unsigned int rprint(Rtype r)
{
	unsigned int p;
	p = ((r.opcode<<26) | (r.rs<<21) | (r.rt<<16) | (r.rd<<11) | (r.funct<<6) | (r.unused));


	return p;
}

unsigned int iprint(Itype i)
{
	unsigned int p;
	if(i.immed >= 0)
		p = ((i.opcode<<26) | (i.rs<<21) | (i.rt<<16) | (i.immed));
	else
		p = ((i.opcode<<26) | (i.rs<<21) | (i.rt<<16) |~ (i.immed)) + 1;

	return p;

}

unsigned int jprint(Jtype j)
{
	unsigned int p;

	p = ((j.opcode<<26) | (j.reg<<25) | (j.address));


	return p;
}

/*the upcoming three functions are initilaizing a struct of the verius commends types*/
static Rtype* rinitial(char *name , int i)
{	
	Rtype *r;
	r = (Rtype *)malloc(sizeof(Rtype));	
	
	r->unused = 0;/*all rtype type unused*/
	r->name = name; 

	if (i <= 5){
		r->opcode = 0;/*First rtype type opcode*/
		r->funct = i+1;/*First rtype type funct*/
	}

	if (i > 5 && i <= 8){
		r->rt = 0;
		r->opcode = 1;/*Seconed rtype type opcode*/
		r->funct = i-4;/*Seconed rtype type funct */  
	}

	return r;
	
}

static Itype* iinitial(char *name , int k)
{	
	Itype *i;
	i = (Itype *)malloc(sizeof(Itype));

	i->name = name;  
	i->opcode = k+iopcode;
		  
	
	return i;
	
}

static Jtype* jinitial(char *name , int k)
{
	Jtype *j;
	j = (Jtype *)malloc(sizeof(Jtype));

	j->name = name;

	switch(k){
		/*jmp*/		
		case 1:
			j->opcode = k+jopcode;
		break;
		/*la*/
		case 2:
			j->opcode = k+jopcode;
			j->reg = (char)0;
		break;
		/*call*/
		case 3:
			j->opcode = jopcode;
			j->reg = (char)0;
		break;
		/*stop*/
		case 4:
			j->opcode = jopcode;
			j->reg = (char)0;
			j->address = 0;	
		break;
	}
	
	return j;
	
}

/*this function is calculating the type of commend in the string*/
int findtype(char *ptr)
{
	if(isrtype(ptr))
		return RTYPE;
	else if(isitype(ptr))
		return ITYPE;
	else if(isjtype(ptr))
		return JTYPE;
	else
		printf("wrong commend");
	
	return ERROR;
}

unsigned int codetoBcode(char *line, int typenum)
{
	int n = 0, k = 0, temp;	
	char *ptr = (char *)malloc(sizeof(char));
	Rtype *R;
	Itype *I;
	Jtype *J;

	enum codetype type;


	while (isalpha(*(line+n)))
		n++;
	strncat(ptr, line, n);/*concat the command to ptr*/
	line += n;
	n = 0;

	type = typenum;

	switch(type){
		case RTYPE:
			n = isrtype(ptr);
			
			R = rinitial(ptr, n);/*Initialize the code by his type from the command table*/

			if (n <= 5){
				while(k < 3){
					ptr = strchr(line, '$');
					ptr++;
					while (!isdigit(*(ptr)))
						ptr++;
					
					temp = atoi(ptr);

					if(k == 0)
						R->rs = temp;
					else if(k == 1)
						R->rt = temp;
					else if(k == 2)
						R->rd = temp;
					line = ptr;
					k++;					
				}
			}				
			else{
				while(k < 2){
					ptr = strchr(line, '$');
					ptr++;
					while (!isdigit(*(ptr)))
						ptr++;
					
					temp = atoi(ptr);

					if(k == 0)
						R->rs = temp;
					else if(k == 1)
						R->rd = temp;
					line = ptr;
					k++;					
				}	
			}	

			
			return rprint(*R);	
		break;
		
		case ITYPE:
			n = isitype(ptr);
		
			I = iinitial(ptr, n);/*Initialize the code by his type from the command table*/

			while(k < 3){
				if(k == 0 || k == 2){
					ptr = strchr(line, '$');
					if (ptr != NULL){
						ptr++;
						while (!isdigit(*(ptr)) && ptr != NULL)
						ptr++;
						temp = atoi(ptr);
					}
					else
						ptr = line;
						ptr = strchr(line, ',');
						if(ptr == NULL)
							return ERROR;
						while(!isalpha(*(ptr)) && ptr != NULL)
							ptr++;
						if(ptr == NULL)
							return ERROR;
						else{
							I->rt = 0;
							k++;
							break;
						}
				}
				else{
					ptr = strchr(line, ',');
					ptr++;
					while (!isdigit(*(ptr)) && *ptr != '-')
					ptr++;
				}
					
				if(k == 0)
					I->rs = temp;
				else if(k == 1){
					if(n <= 5 || n > 9){
						temp = atoi(ptr);
						I->immed = (short)temp;
					}
					else if(n > 5 && n <= 9){
						I->immed = 0;
					}
				}
				else if(k == 2)
					I->rt = temp;
				line = ptr;
				k++;
			}						
		
			return iprint(*I);	
		break;
		
		case JTYPE:
			n = isjtype(ptr);

			J = jinitial(ptr, n);/*Initialize the code by his type from the command table*/
			ptr = line;
			while(isspace(*ptr)){
				if(*ptr != '\0')
					ptr++;
				else
					break;
			}
			if(n == 1 && *ptr == '$'){/*jmp*/
				J->reg = 1;
				while (!isdigit(*(ptr)))
					ptr++;
				temp = atoi(ptr);
				J->address = temp;
			}
			else if(n <= 3)
				J->address = 0;

			return jprint(*J);	
		break;
		
		default:
			return 0;
	}
	
	return 0;
}

