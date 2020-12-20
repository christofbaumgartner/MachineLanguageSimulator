#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* This is the number of addresses that can be used in the machine program for memory and stack. (S1, S2, S3, D1, D2, etc) */
#define MAX_NO_OF_ADDRESSES 50
/* This is the maximum length that the address can have (e.g. S99 would be valid, S100 would not) */
#define MAX_ADDRESS_SIZE 4
/* This is the maximum content that can be saved under one address */
#define MAX_ADDRESS_CONTENT 10
/* Max length for a line of pseudo machine code */
#define MAX_INPUT_LINE_LENGTH 22

int flush_buffer(void);
int process_line(char line[]);

int main(int argc, char * argv[]){
	
	
	int i = 0, j;
	char c;
	char cmdline_input[MAX_NO_OF_ADDRESSES * MAX_INPUT_LINE_LENGTH];
	char line[MAX_INPUT_LINE_LENGTH];
	char help1[] = "-h";
	char help2[] = "--help";
	
	
	struct mcode_line{
		
		int step[MAX_ADDRESS_SIZE];
		char command[MAX_ADDRESS_CONTENT]; 
		int val1[MAX_ADDRESS_SIZE];
		int val2[MAX_ADDRESS_SIZE];
	};
	
	struct mcode_line mcode[MAX_NO_OF_ADDRESSES];
	
	
		
	printf("Enter your pseudo machine code. Type '!' when done.\n\n");
	
	c = getchar();
	while (c != '\0' && i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) -1){
		if (c != '\0' && c != EOF && c != '!'){
			cmdline_input[i++] = c;
			c = getchar();
		} else {
			cmdline_input[i++] = '\0';
		}
	}
	
	printf("Print back input. Marker 2\n");
	
	/* DEBUG START: Print back Code*/
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) - 1; i++){
		if (cmdline_input[i] != '\0'){
			printf("%c", cmdline_input[i]);
		} 
	}
	/* DEBUG END*/
	
	
	printf("Split line. Marker 3\n");
	i = 0;	
	while (cmdline_input[i] != '\n' && cmdline_input[i] != '\0'){
		line[i] = cmdline_input[i];
		i++;
	}
	line[i] = '\0';
			
	printf("Print back line. Marker 4\n");
	
	/* DEBUG START: Print back line */ 
	printf("\n");
	for (j = 0; j < (MAX_INPUT_LINE_LENGTH) - 1; ++j){
		if (line[j] != '\0'){
			printf("%c", line[j]);
		} else { 
			continue;
		}
	}
	/* DEBUG END*/
		
	printf("\nEnd. Marker 5\n");
	
	
	if ((argc == 2 && strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)){
		printf("\n Help:\n\n Pass your pseudo machine code file: %s.exe < yourcode.mc\n\n An example for pseudo code is:\n\n", argv[0]);
		printf(" P1 INIT \tS2\n P2 INCREMENT \tS2\n P3 JUMP0 \tP7,S1\n P4 ADD \tS2,S2\n P5 DECREMENT \tS1\n P6 JUMP \tP3\n P7 RETURN \tS2\n S1 \t\t2\n S2\n");
		return 0;
	}
	
	return 0;
	
}


int flush_buffer(void){
	int c;
	while ((c = getchar()) != EOF){}
	return c != EOF;
}


int process_line(char line[]){
	
	
	return 0;
}
