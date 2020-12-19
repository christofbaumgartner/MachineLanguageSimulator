#include <stdio.h>
#include <string.h>
#include "interpreter.h"

int main(void){
	
	printf("Starting Machine Code Simulator\n\n");
	/* printf("\r"); */
	printf("Loaded Program with status code: %i\n", load_program());
	printf("Executed Program with status code %i\n", execute_program());
	return 0;
}