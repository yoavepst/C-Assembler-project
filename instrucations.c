/*This file collects all functions relted to assembler insructions - a non commads data*/

#include "assembler.h"

enum instruction {DB, DH, DW, ASCIZ};
static const char  *instructionsname[] = {".db", ".dh", ".dw", ".asciz"}; 

/*returns the data type*/
int finddata(char *ptr)
{
	int find;

	if(!strcmp(instructionsname[DB], ptr))
		find = DB;
	else if(!strcmp(instructionsname[DH], ptr))
		find = DH;
	else if(!strcmp(instructionsname[DW], ptr))
		find = DW;
	else if(!strcmp(instructionsname[ASCIZ], ptr))
		find = ASCIZ;
	else
		find = ERROR;
	
	
	return find;
}

/*recieve a assemler line, and an already gifured data type, and structs it into union*/
INS *datatostruct(char *line, int datatype)
{
	INS *in = (INS *)malloc(sizeof(INS));
	char *ptr = (char *)malloc(sizeof(char));
	int j = 0;
	enum instruction data;

	data = datatype;
	in->validmemeber = datatype;/*points the relevant memeber of the union*/
	ptr = &line[j];
	
	switch(data){
		case DB:
			in->member.c = (signed char *)malloc(sizeof(signed char));
			while(*ptr != '\0'){
				if (*(in->member.c) != 0)
					in->member.c = (signed char *)realloc(in->member.c, j+1);
				while(!isdigit(*ptr) && (*ptr) != '-')/*point the next number*/
					ptr++;
				*((in->member.c) + j) |= (signed char)atoi(ptr);
				while(*ptr != ',' && *ptr != '\0')
					ptr++;
				j++;
			}
			*((in->member.c) + j) = '\0'; 
		break;
		
		case DH:
			in->member.s = (signed short *)malloc(sizeof(signed short));
			while(*ptr != '\0'){
				if (*(in->member.s) != 0)
					in->member.s = (signed short *)realloc(in->member.s, j+1);
				while(!isdigit(*ptr) && (*ptr) != '-')/*point the next number*/
					ptr++;
				*((in->member.s) + j) |= (signed short)atoi(ptr);
				while(*ptr != ',' && *ptr != '\0')
					ptr++;
				j++;
			}
			*((in->member.s) + j) = '\0';  
		break;

		case DW:
			in->member.i = (signed int *)malloc(sizeof(signed int));
			while(*ptr != '\0'){
				if (*(in->member.i) != 0){
					in->member.i = (signed int *)realloc(in->member.i, j+1);
					*((in->member.i) + j) &= 0;
				}
				while(!isdigit(*ptr) && (*ptr) != '-')
					ptr++;
				*((in->member.i) + j) |= (signed int)atoi(ptr);
				while(*ptr != ',' && *ptr != '\0')
					ptr++;
				j++;
			}
			*((in->member.i) + j) = '\0';  
		break;

		case ASCIZ:
			in->member.str = (char *)malloc(sizeof(char));
			while (*ptr != '"')
				ptr++;
			ptr++;/*point on the first letter*/
			while(*ptr != '"'){
				if (*(in->member.str) != 0)
					in->member.str = (char *)realloc(in->member.str, j+1);
				if(isgraph(*ptr))
					strncat(in->member.str, ptr, sizeof(char));
				ptr++;
				j++;
			}
			*((in->member.i) + j) = '\0'; 
		break;
	}
	return in;
}

/*calculte the amount of data that was struncted from an assembler line*/
int datalength (INS *in)
{
	int length = 0;
	switch(in->validmemeber){
		case 0:
			while(*((in->member.c) + length) != '\0')
				length++;	
		break;

		case 1:
			while(*((in->member.s) + length) != '\0')
				length++;
		break;

		case 2:
			while(*((in->member.i) + length) != '\0')
				length++;
		break;

		case 3:
			length = strlen(in->member.str);
		break;

		default:
			length = '\0';
		break;
	}

	return length;
}

/*tranform each member to a binary code*/
int datatoBcode (INS *in, int index) 
{
	int bcode;
	switch(in->validmemeber){
		case 0:
			bcode = (int)*((in->member.c)+index);
		break;

		case 1:
			bcode = (int)*((in->member.s)+index);
		break;

		case 2:
			bcode = (int)*((in->member.i)+index);
		break;

		case 3:
			bcode = (int)*((in->member.str)+index);
		break;

		default:
			bcode = '\0';
		break;
	}

	return bcode;
}
