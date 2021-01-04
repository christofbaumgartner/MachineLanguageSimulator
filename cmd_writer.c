#include "cmd_writer.h"

struct mcode_line mcode[MAX_NO_OF_ADDRESSES];

int print_help(char *filename)
{
	printf("\n Help:\n\n Type your pseudo machine code or pass a file like this: %s < yourcode.mc\n\n Valid commands are:\n\n", filename);
	printf(" INIT      \tA\tStore value 0 at address A.\n");
	printf(" ADD       \tA,B\tAdd to memory address A the value of memory address B.\n");
	printf(" SUB       \tA,B\tSubstract from memory address A the value of memory address B.\n");
	printf(" DECREMENT \tA\tDecrease the value of memory address A by 1.\n");
	printf(" DECREMENT0\tA,B\tDecrease the value of memory address A by 1 in case value of memory address B equals 0.\n");
	printf(" INCREMENT \tA\tIncrease the value of memory address A by 1.\n");
	printf(" INCREMENT0\tA,B\tIncrease the value of memory address A by 1 in case value of memory address B equals 0.\n");	
	printf(" JUMP      \tA\tJump to program pointer A.\n");
	printf(" JUMP0     \tA,B\tJump to program pointer A in case value of memory address B equals 0.\n");
	printf(" RETURN    \tA\tReturn value of memory address A.\n");
	printf(" RETURN0   \tA,B\tReturn value of memory address A in case value of memory address B equals 0.\n\n");
	printf(" PROGRAM step has the format Px while 'x' is the line number.\n DATA address has the format Dx while 'x' is the memory address.\n"); 
	printf(" STACK address has the format Sx while 'x' is the memory address.\n HEAP address has the format Hx while 'x' is the memory address.\n\n");
	printf(" Addresses of successive memory cells are numbered consecutively. Do not leave any gaps.\n\n");
	printf(" Separation either by 'Space', 'Tab' or ','.\n\n");
	printf(" Example code:\n\n P1\tINIT     \tS2\n P2\tINCREMENT \tS2\n P3\tJUMP0   \tP7,S1\n P4\tADD     \tS2,S2\n P5\tDECREMENT\tS1\n P6\tJUMP    \tP3\n P7\tRETURN  \tS2\n S1\t2\n S2\n");
	return 0;
}

int print_code_promt()
{
	printf("\nEnter your pseudo machine code or '?' for help. Type '!' in a new line when done.\n\n");
	return 0;
}

/*Print back Code Input*/
int print_back_input(char *cmdline_input)
{
	int i;
	printf("Your code input: \n\n");
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) - 1; i++){
		if (cmdline_input[i] != '\0'){
			printf("%c", cmdline_input[i]);
		} 
	}
	printf("\n");
	return 0;
}

int print_structured_code(int line_count)
{
	int i;
	printf("\nMACHINE CODE:\n\n");
	for (i = 0; i < line_count; i++){
		printf("Line: %i \t", i + 1);
		printf("RegType: %c\t", mcode[i].regtype);
		printf("IsCommand: %i\t", mcode[i].iscommand);
		printf("step: %i \t", mcode[i].step);
		printf("command: %s \t", mcode[i].command);
		printf("type1: %c\t", mcode[i].type1);
		printf("val1: %i  \t", mcode[i].val1);
		printf("type2: %c\t", mcode[i].type2);
		printf("val2: %i  \n", mcode[i].val2);
	}
	return 0;
}