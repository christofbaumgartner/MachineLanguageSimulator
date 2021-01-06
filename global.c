#include "global.h"

#include <stdio.h>
#include <limits.h>

int flush_buff(void);
int read_buff(void);

int flush_buff(void){

	int c;
	while ((c = getchar()) != '\n' && (c != EOF))
	{}
	return c != EOF;
	/* returns 0 for buffer error, else 1 */
}


int read_buff(void){
	
	int num_input;
	int status;
	int c = '\0';
		
	status = scanf("%i", &num_input);
	printf("Num is %i\n", num_input);
	
	if (status == EOF){
		error = 14;
		return INT_MIN;
	}
	if (status != 1 || num_input < (INT_MIN + 1) || (c = getchar()) != '\n'){
		if (c == EOF || !flush_buff()){
			error = 14;
			return INT_MIN;
		}
		error = 15;
		return INT_MIN;	
	} 
	
	return num_input;
}
