#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAXLINES 80
#define MAXTAG 31
#define MAXSTR 11
#define ERROR -1

#define RTYPENUM 8
#define ITYPENUM 15
#define JTYPENUM 4

enum attributes {EXTERNAL, ENTRY, CODE, DATA};
enum codetype{RTYPE, ITYPE, JTYPE};

typedef struct rtype{
	char *name;	
	unsigned int unused;	
	unsigned int funct;
	unsigned int rd;
	unsigned int rt;
	unsigned int rs;
	unsigned int opcode;
}Rtype;

typedef struct itype{
	char *name;	
	int immed;	
	unsigned int rt;
	unsigned int rs;
	unsigned int opcode;
}Itype;

typedef struct jtype{
	char *name;	
	int address;	
	unsigned char reg;
	unsigned int opcode;
}Jtype;

typedef union instruct{
	signed char *c;
	signed short *s;
	signed int *i;
	char *str;
}UINS;

typedef struct instructions{
	int validmemeber;
	UINS member;
}INS;

typedef struct bcode{
	int address;
 	unsigned int bcode;	
}Bcode;

typedef struct bnode {
    Bcode b;
    struct bnode * next;
}Bcode_node;

typedef struct snode {
	char *symbol;
	int value;
	enum attributes att;
    struct snode * next;
}Symbol_node;

typedef struct first {
	Bcode_node *IC;
	Bcode_node *DC;  
	Symbol_node *Symbol; 
}Firsttrans;

typedef struct exter{
	char *symbol;
	unsigned int value;
    struct exter * next;
}Extern_node;

typedef struct second {
	Bcode_node *ob;
	Extern_node *ext; 
	Symbol_node *ent; 
}Sectrans;





