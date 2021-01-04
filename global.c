#include "global.h"


int flush_buff(void){

	int c;
	while ((c = getchar()) != '\0' && (c != EOF) && (c != '\n'))
	{}
	return c != EOF;
	/* returns 0 for buffer error, else 1 */
}


int read_buff(void){
	
	int num, status;
	int c = '\0';
	status = scanf("%i", &num);
	
	if (status == EOF){
		error = 14;
		return INT_MIN;
	}
	if (status != 1 || num < (INT_MIN + 1) || (c = getchar ()) != '\n'){
		if (c == EOF || !flush_buff()){
			error = 14;
			return = INT_MIN;
		}
		error = 15;
		return INT_MIN;	
	} 
	
	return num;
}