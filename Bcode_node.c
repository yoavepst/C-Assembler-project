/*
	This progrem is exacuting the binarry code and adderses table.
*/

#include "assembler.h"
/*boolean yes or no is the node is empty*/
int isemptybnode(Bcode_node *node)
{
	if(node == NULL || (node -> b.bcode == 0 && node -> b.address ==  0 && node -> next == NULL))
		return 1;
	return 0;
}

/*push another node to the end of the memory list*/
int mempush(int binarycode, Bcode_node *head, int value)
{
	Bcode_node *current;
	current = head;

	while (current -> next != NULL)
		current = current -> next;  		
	
	if(isemptybnode(current)){/*first on the list*/
		current -> b.bcode = binarycode;
		current -> b.address =  value;
	}
	else{
	current -> next = (Bcode_node *)malloc(sizeof(Bcode_node));
	current = current -> next;
	current -> b.bcode = binarycode;
	current -> b.address =  value;
	}
		
	return 1;

}

/*merging two bnode lists*/
void mergelist (Bcode_node *head, Bcode_node *tail)
{
	Bcode_node *current;
	current = head;

	while (current -> next != NULL)
		current = current -> next;

	current -> next = tail;
}

/*get the node's address*/
int getaddress (Bcode_node *bnode)
{
	if (bnode -> b.address != ERROR)
		return bnode -> b.address;
	return ERROR;
}

/*get the node's binary code*/
int getbcode (Bcode_node *bnode)
{
	if (bnode -> b.bcode != ERROR)
		return bnode -> b.bcode;
	return ERROR;
}
