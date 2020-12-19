#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

/* This is the number of addresses that can be used in the machine program for memory and stack. (S1, S2, S3, D1, D2, etc) */
#define MAX_NO_OF_ADRESSES 50
/* This is the maximum length that the address can have (e.g. S99 would be valid, S100 would not) */
#define MAX_ADRESS_SIZE 4
/* This is the maximum content that can be saved under one adress */
#define MAX_ADRESS_CONTENT 10


static char adress_labels[MAX_NO_OF_ADRESSES][MAX_ADRESS_SIZE];
static char adress_values[MAX_NO_OF_ADRESSES][MAX_ADRESS_CONTENT];

int load_program(void){
	
	struct mcode_line
		{
		char step[MAX_ADRESS_SIZE];
		char command[MAX_ADRESS_CONTENT]; 
		char val1[MAX_ADRESS_SIZE];
		char val1[MAX_ADRESS_SIZE];
	}
	struct mcode_line mcode[MAX_NO_OF_ADRESSES];
	
	char testadress[] = "P1";
	char testcontent[] = "INIT S2";
	strcpy(adress_labels[0], testadress);
	strcpy(adress_values[0], testcontent);
	return 0;
}

int execute_program(void){
	return 0;
}