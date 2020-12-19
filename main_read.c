#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* This is the number of addresses that can be used in the machine program for memory and stack. (S1, S2, S3, D1, D2, etc) */
#define MAX_NO_OF_ADDRESSES 50
/* This is the maximum length that the address can have (e.g. S99 would be valid, S100 would not) */
#define MAX_ADDRESS_SIZE 4
/* This is the maximum content that can be saved under one address */
#define MAX_ADDRESS_CONTENT 10

int flush_buffer(void);


int main(int argc, char * argv[]){
	
	char help1[] = "-h";
	char help2[] = "--help";
	
	struct mcode_line{
		
		int step[MAX_ADDRESS_SIZE];
		char command[MAX_ADDRESS_CONTENT]; 
		int val1[MAX_ADDRESS_SIZE];
		int val2[MAX_ADDRESS_SIZE];
	};
	
	struct mcode_line mcode[MAX_NO_OF_ADRESSES];
	
	
	if ((strcmp(argv[1], help1) == 0) || (strcmp(argv[1], help2) == 0)){
		printf("\n Help:\n\n Pass your pseudo machine code file: %s.exe < yourcode.mc\n\n An example for pseudo code is:\n\n", argv[0]);
		printf(" P1 INIT \tS2\n P2 INCREMENT \tS2\n P3 JUMP0 \tP7,S1\n P4 ADD \tS2,S2\n P5 DECREMENT \tS1\n P6 JUMP \tP3\n P7 RETURN \tS2\n S1 \t\t2\n S2\n");
		return 0;
	}
	
	printf("Debug: Number of arguments passed: %i", argc);

	
	
	return 0;
}


int flush_buffer(void){
	int c;
	while ((c = getchar()) != EOF){}
	return c != EOF;
}