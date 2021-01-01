#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include "config.h"

struct mcode_line{
		char regtype;
		int iscommand;
		int step;
		char command[MAX_ADDRESS_CONTENT]; 
		char type1;
		int val1;
		char type2;
		int val2;
	};

extern int *stack, *data, *heap;

#endif