#include <stdio.h>
#include <string.h>
#include "interpreter.h"

int main(int argc, char * argv[]){
	
	char help1[] = "-h";
	char help2[] = "--help";
	
	if ((strcmp(argv[1], help1) == 0) || (strcmp(argv[1], help2) == 0)){
		printf("Help:\n\n To run your pseudo machine code pass a file to this program with: THISPROGRAM < yourcode.mc\n\n An example for pseudo code is:\n\n ....");
		return 0;
	}	
	
	printf("Starting Machine Code Simulator\n\n");
	/* printf("\r"); */
	printf("Loaded Program with status code: %i\n", load_program());
	printf("Executed Program with status code %i\n", execute_program());
	return 0;
}