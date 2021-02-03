#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include "config.h"
#include <stdio.h>
#include <limits.h>

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

struct mcode_line mcode[MAX_NO_OF_ADDRESSES];
extern int *stack, *data, *heap;
extern int error;

int flush_buff(void);
int read_buff(void);

#endif