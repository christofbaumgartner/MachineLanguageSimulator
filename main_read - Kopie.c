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

#define ADD 'a';
#define ADD0


struct mcode_line{
		char regtype;
		int step;
		char command[MAX_ADDRESS_CONTENT]; 
		char type1;
		int val1;
		char type2;
		int val2;
	};
	
struct mcode_line mcode[MAX_NO_OF_ADDRESSES];


int process_line(int char_count, int line_count, char line[]);


int main(int argc, char * argv[]){
	
	
	int i = 0, j = 0, k = 0, char_count = 0, file_check, line_count = 0;
	char c;
	char cmdline_input[MAX_NO_OF_ADDRESSES * MAX_INPUT_LINE_LENGTH];
	char line[MAX_INPUT_LINE_LENGTH];
		
		
	i = 0;
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

	

	/* Split to single lines and process */
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) - 1; i++){
		if (cmdline_input[i] != '\n'){
			line[j] = cmdline_input[i];
			char_count++;
			j++;
		/*} else if (cmdline_input[i] == '\0' && cmdline_input[i + 1] == '\0'){ 
			break;*/
		} else {
			line[j] = '\0';
			file_check = process_line(char_count, line_count, line);
			j = 0;
			char_count = 0;
			line_count++;
		}
		
	}
	
	
	
	printf("\nParse line: %i. Marker 4\n", file_check);
	/* printf("mcode.step = %d.\n", mcode[line_count].step); */
	
	/* DEBUG START: Print back line */ 
	for (k = 0; k < (MAX_INPUT_LINE_LENGTH) - 1; k++){
		if (line[k] != '\0'){
			printf("%c", line[k]);
		} else if (line[k] =='\0' && line[k + 1] == '\0'){ 
			break;
		} else {
			printf("\n");
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


int process_line(int char_count, int line_count, char line[]){
	
	int i, j, k, l, sum = 0, factor = 1, div_count = 0, iscommand = 0, isval = 1;
	char command[MAX_ADDRESS_CONTENT];
	struct mcode_line temp_line;
	
	/* Extract values and place in struct mcode */
	
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH) - 1; i++){
		if (line[i] != '\0' && i < char_count){
			
			if (line[i] == 'P' || line[i] == 'p'){					
				iscommand = 1;
				sum = 0;
				factor = 1;
				j = i + 1;
				
				/* Parse Step */
				
				while(line[j] != 32 && line[i] != 9){	
				
					if (line[j] > 47 && line[j] < 58){
						
						sum = sum * factor + ((line[j] - 48));
						temp_line.step = sum;
						if (factor == 1){
							factor = 10;
						}
						
						j++;
					
					} else {
					printf("Invalid programm code in line: %i. Aborting. j: %i, %c, i: %i, %c\n", line_count, j, line[j], i, line[i]);
						return 0;
					}
					
					i++;
				}
				div_count++;
				i = i + 2;
				
				/* Parse Command and Values */
			
				if (iscommand == 1){
				
				k = 0;
				
				while(line[i] != 32 && line[i] != 9){
					command[k] = line[i];
					k++;
					i++;
				}
				command[k] = '\0';
												
				strncpy(temp_line.command, command, MAX_ADDRESS_CONTENT); 
				div_count++;
				}
				
				
				sum = 0;
				factor = 1;
				isval = 1;
				
				
				/*
				while(line[i] > 47 && line[i] < 58){
					sum = sum * factor + ((line[i] - 48));
					printf("Sum Val: %i", sum);
					if (factor == 1){
						factor = 10;
					}				
					if (isval == 1){
						temp_line.val1 = sum;
						temp_line.val2 = ' ';
					} else if (isval == 2){
						temp_line.val2 = sum;
					}	
								
				} if (line[i] == 'P' || line[i] == 'D' || line[i] == 'S'){
					if (isval == 1){
						temp_line.type1 = line[i];
						temp_line.type2 = ' ';
					} else if (isval == 2){
						temp_line.type2 = line[i];
					}
						
				} else if (line[i] == ','){
					isval = 2;
					sum = 0;
					factor = 1;
				
				} else {
					printf("Invalid programm code in line: %i. Aborting. %c\n", line_count, line[i]);
					return 0;
				} 
				*/
				
				/* Parse Stack and Progamm Memory */
				
			}
			
			iscommand = 0;
			
			
		} else if (line[i] =='\0' && line[i + 1] == '\0'){ 

			printf("\nParse line done.\n");
			
			/* debug start */
			printf("Line: %i ", line_count + 1);
			printf("step: %i\t", temp_line.step);
			printf("command: %s\t", temp_line.command);
			printf("type1: %c\t", temp_line.type1);
			printf("val1: %i\t", temp_line.val1);
			printf("type2: %c\t", temp_line.type1);
			printf("val2: %i\n", temp_line.val1);
			/* debug end */
						
			mcode[line_count] = temp_line;
			return 1;
		} 
	
		
	}
	
	return 0;
}
