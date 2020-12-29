#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "types.h"
#include "simulator.h"
	
struct mcode_line mcode[MAX_NO_OF_ADDRESSES];
int *stack, *data, *heap;

int process_line(int char_count, int line_count, char line[]);
int sum_array(char sline[]);
int get_mem_size(char mtype, int line_count); 
void fill_mem_array(int array[], char mtype, int line_count);

int main(int argc, char * argv[]){
	
	int i = 0, j = 0, char_count = 0, file_check = 1, line_count = 0, error = 0;
	char c;
	char cmdline_input[MAX_NO_OF_ADDRESSES * MAX_INPUT_LINE_LENGTH];
	char line[MAX_INPUT_LINE_LENGTH];
	

	/* help section */
	
	if ((argc == 2) && ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0))){
		help:
			printf("\n Help:\n\n Type your pseudo machine code or pass a file like this: %s.exe < yourcode.mc\n\n Valid commands are:\n\n", argv[0]);
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
			printf(" PROGRAM address has the format Px while 'x' is the line number.\n DATA address has the format Dx while 'x' is the line number.\n"); 
			printf(" STACK address has the format Sx while 'x' is the line number.\n HEAP address has the format Hx while 'x' is the line number.\n\n");
			printf(" Separation either by 'Space', 'Tab' or ','.\n\n");
			printf(" Example code:\n\n P1\tINIT     \tS2\n P2\tINCREMENT \tS2\n P3\tJUMP0   \tP7,S1\n P4\tADD     \tS2,S2\n P5\tDECREMENT\tS1\n P6\tJUMP    \tP3\n P7\tRETURN  \tS2\n S1\t2\n S2\n");
			return 0;
	}
		
	i = 0;
	printf("\nEnter your pseudo machine code or '?' for help. Type '!' when done.\n\n");
	c = getchar();
	
	/* get input from stdin */
	
	while (c != '\0' && i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) -1){
		if(c == '?'){
			goto help;
		
		} else if (c != '\0' && c != EOF && c != '!'){
			cmdline_input[i++] = c;
			c = getchar();
		} else {
			cmdline_input[i++] = '\0';
		}
	}
	
	printf("Your code input: \n\n");
	
	/*Print back Code Input*/
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) - 1; i++){
		if (cmdline_input[i] != '\0'){
			printf("%c", cmdline_input[i]);
		} 
	}
	
	

	/* Split to single lines and process */
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) - 1; i++){
		if (cmdline_input[i] != '\n'){
			line[j] = cmdline_input[i];
			char_count++;
			j++;
		
		} else {
			line[j] = '\0';
			file_check = process_line(char_count, line_count, line);
			if (file_check == 0){
				error = -1;
			}
			j = 0;
			char_count = 0;
			line_count++;
		}
		
	}
	

	/* print back processed mcode */
	
	printf("\nMACHINE CODE:\n\n");
	
	for (i = 0; i < line_count; i++){
		
		printf("Line: %i \t", i + 1);
		printf("RegType: %c\t", mcode[i].regtype);
		printf("IsCommand: %i\t", mcode[i].iscommand);
		printf("step: %i \t", mcode[i].step);
		printf("command: %s      \t", mcode[i].command);
		printf("type1: %c\t", mcode[i].type1);
		printf("val1: %i  \t", mcode[i].val1);
		printf("type2: %c\t", mcode[i].type2);
		printf("val2: %i  \n", mcode[i].val2);
	}
	
	
	/* fill stack, data and heap arrays from mcode[]*/
	
	/* stack */
	stack = malloc(get_mem_size('S', line_count) * sizeof(int));
	
	if (stack == NULL) {
		printf("Memory could not be allocated.");
		return 1;
	}
	
	fill_mem_array(stack, 'S', line_count);
		

	/* data */
	
	data = malloc(get_mem_size('D', line_count) * sizeof(int));
	
	if (data == NULL) {
		printf("Memory could not be allocated.");
		return 1;
	}
	
	fill_mem_array(data, 'D', line_count);

	
	/* heap */
	
	heap = malloc(get_mem_size('H', line_count) * sizeof(int));
	
	if (heap == NULL) {
		printf("Memory could not be allocated.");
		return 1;
	}
	
	fill_mem_array(heap, 'H', line_count);

	
	/* print back processing of mcode */

	printf("\nAUSFUEHRUNG:\n\n");
	execute_program();

	for (i = 0; i < (get_mem_size('H', line_count)); i++){
		printf("H%i: %i\n", i + 1, heap[i]);
	}
		
	for (i = 0; i < (get_mem_size('D', line_count)); i++){
		printf("D%i: %i\n", i + 1, data[i]);
	}

	for (i = 0; i < (get_mem_size('S', line_count)); i++){
		printf("S%i: %i\n", i + 1, stack[i]);
	}

	
	free(heap);
	free(stack);
	free(data);

	(error == 0) ? printf("\nExitCode: OK\n") : printf("\nExitCode: ERROR\n");
	return 0;
	
}


int process_line(int char_count, int line_count, char line[]){
	
	/* to verify commands: */
	struct coms {
		char command[12];
	};
	
	struct coms v_coms[11] = {
		{"INIT"},
		{"ADD"},
		{"SUB"},
		{"DECREMENT"},
		{"DECREMENT0"},
		{"INCREMENT"},
		{"INCREMENT0"},
		{"JUMP"},
		{"JUMP0"},
		{"RETURN"},
		{"RETURN0"}
	};
	

	int i, j = 1, k = 0, l, div_count = 0;
	char command[MAX_ADDRESS_CONTENT];
	char temp[MAX_ADDRESS_CONTENT];
	struct mcode_line temp_line;
	
	/* Extract values and place in struct mcode */
	
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH) - 1; i++){
		if (line[i] != '\0' && i < char_count){
			
			/* Check first charakter to determine if program or memory */
			
			if ((i == 0) && (line[i] == 'P' || line[i] == 'p')){	
				temp_line.iscommand = 1;
				temp_line.regtype = 'P';
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[j - 1] = line[j];
					j++;
				}
				temp[j - 1] = '\0';
				temp_line.step = sum_array(temp);
				div_count++;
										
			} else if ((i == 0) && (line[i] == 'D' || line[i] == 'd')){	
				temp_line.iscommand = 0;
				temp_line.regtype = 'D';
				strcpy(temp_line.command, "0");
				temp_line.step = -1;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[j - 1] = line[j];
					j++;
				} 
				temp[j - 1] = '\0';
				temp_line.type1 = '0';
				temp_line.val1 = sum_array(temp);
				j++;
				l = 0;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[l] = line[j];
					j++;
					l++;
				} 
				temp[j - 1] = '\0';
				temp_line.type2 = '0';
				temp_line.val2 = sum_array(temp);
				
			} else if ((i == 0) && (line[i] == 'S' || line[i] == 's')){	
				temp_line.iscommand = 0;
				temp_line.regtype = 'S';
				strcpy(temp_line.command, "0");
				temp_line.step = -1;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[j - 1] = line[j];
					j++;
				} 
				temp[j - 1] = '\0';
				temp_line.type1 = '0';
				temp_line.val1 = sum_array(temp);
				j++;
				l = 0;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[l] = line[j];
					j++;
					l++;
				} 
				temp[j - 1] = '\0';
				temp_line.type2 = '0';
				temp_line.val2 = sum_array(temp);
				
					
								
			} else if ((i == 0) && (line[i] == 'H' || line[i] == 'h')){	
				temp_line.iscommand = 0;
				temp_line.regtype = 'H';
				strcpy(temp_line.command, "0");
				temp_line.step = -1;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[j - 1] = line[j];
					j++;
				} 
				temp[j - 1] = '\0';
				temp_line.type1 = '0';
				temp_line.val1 = sum_array(temp);
				j++;
				l = 0;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[l] = line[j];
					j++;
					l++;
				} 
				temp[j - 1] = '\0';
				temp_line.type2 = '0';
				temp_line.val2 = sum_array(temp);
				
				
			} else if (i == 0){
				printf("\nInvalid programm code in line: %i.\n", line_count + 1);
				return 0;
			}
			
			/* extract command */
						
			if ((temp_line.iscommand == 1) && (div_count == 1)){
				
				for (l = 0; l < div_count; l++){
					j = 0;
					while(line[j] != 32 && line[j] != 9 && line[j] != 44){
						j++;
					}
					j++;
				}
				
				div_count++;
				k = 0;
				
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){
					command[k] = line[j];
					k++;
					j++;
				}
				command[k] = '\0';
												
				strncpy(temp_line.command, command, MAX_ADDRESS_CONTENT); 
				
				/* verify if command is valid */
				
				for (k = 0; k < 11; k++){
					if (strcmp(temp_line.command, v_coms[k].command) == 0){
						break;
					} else if ((strcmp(temp_line.command, v_coms[k].command) != 0) && (k == 10)) {
						printf("\nInvalid command in line: %i\n", line_count + 1);
						return 0;
					}
				}
				
				
			}
			
			/* extract type and value 1 */ 
			
			if ((temp_line.iscommand == 1) && (div_count == 2)){
				j = 0;
				
				for (l = 0; l < div_count; l++){
					while(line[j] != 32 && line[j] != 9 && line[j] != 44){
						j++;
					}
					j++;
				}
						
				
				if ((line[j] == 'P' || line[j] == 'p')){	
					temp_line.type1 = 'P';			
										
				} else if ((line[j] == 'D' || line[j] == 'd')){	
					temp_line.type1 = 'D';
				
				} else if ((line[j] == 'S' || line[j] == 's')){	
					temp_line.type1 = 'S';
									
				} else if ((line[j] == 'H' || line[j] == 'h')){	
					temp_line.type1 = 'H';
					
				} else {
					printf("Invalid programm code in line: %i.\n", line_count);
					return 0;
				}
				
				
				k = 0;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44 && line[j] != '\0'){	
					temp[k] = line[j + 1];
					j++;
					k++;
				}
				temp[k] = '\0';
				temp_line.val1 = sum_array(temp);
				div_count++;
			}
			
			/* extract type and value 2 */ 
			
			if ((temp_line.iscommand == 1) && (div_count == 3)){
				j = 0;
				
				for (l = 0; l < div_count; l++){
					while(line[j] != 32 && line[j] != 9 && line[j] != 44){
						j++;
					}
					j++;
				}
				
				
				if ((line[j] == 'P' || line[j] == 'p')){	
					temp_line.type2 = 'P';
					
				} else if ((line[j] == 'D' || line[j] == 'd')){	
					temp_line.type2 = 'D';
					
				} else if ((line[j] == 'S' || line[j] == 's')){	
					temp_line.type2 = 'S';
				
				} else if ((line[j] == 'H' || line[j] == 'h')){	
					temp_line.type2 = 'H';				
				
				} else {
					temp_line.type2 = '0';
					temp_line.val2 = -1;
				}
				
				
				k = 0;
				while(line[j] != 32 && line[j] != 9 && line[j] != 44 && line[j] != '\0'){	
					temp[k] = line[j + 1];
					j++;
					k++;
				}
				temp[k] = '\0';
				temp_line.val2= sum_array(temp);
				div_count++;
			}
		
			
		} else if (line[i] =='\0' && line[i + 1] == '\0'){ 

			/* DEBUG START: Shows Error in Lines 
			printf("Line: %i\t", line_count + 1);
			printf("RegType: %c\t", temp_line.regtype);
			printf("IsC: %i\t", temp_line.iscommand);
			printf("step: %i\t", temp_line.step);
			printf("command: %s\t", temp_line.command);
			printf("type1: %c\t", temp_line.type1);
			printf("val1: %i\t", temp_line.val1);
			printf("type2: %c\t", temp_line.type2);
			printf("val2: %i\n", temp_line.val2);
			DEBUG END */
						
			mcode[line_count] = temp_line;
			return 1;
		} 
	
	}
	return 0;
	
}


int sum_array(char line[]){
	
	int i = 0, sum = 0, factor = 1;
	
	if (line[i] > 47 && line[i] < 58 && line[i] != '\0'){
		while (line[i] > 47 && line[i] < 58 && line[i] != '\0'){
			sum = sum * factor + ((line[i] - 48));
			if (factor == 1){
				factor = 10;
			}
			i++;
		}
	
		return sum;
		
	} else {
		return -1;
	}
}


int get_mem_size(char mtype, int line_count){
	
	int i;
	int size = 0;
	
	for (i = 0; i < line_count; i++){
		if (mcode[i].regtype == mtype){
			size++;
		}
	} 
	
	return size;
}


void fill_mem_array(int array[], char mtype, int line_count){
	
	int i;
		
	for (i = 0; i < line_count; i++){
		if (mcode[i].regtype == mtype){
			array[(mcode[i].val1 - 1)] = mcode[i].val2;
		}
	}
		
}