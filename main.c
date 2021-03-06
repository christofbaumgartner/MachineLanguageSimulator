#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "config.h"
#include "global.h"
#include "simulator.h"
#include "cmd_writer.h"
	
struct mcode_line mcode[MAX_NO_OF_ADDRESSES];
int *stack, *data, *heap;
int memsizes[3];
int error = 0;

int process_line(int char_count, int line_count, char line[]);
int sum_array(char sline[]);
int get_mem_size(char mtype, int line_count); 
void fill_mem_array(int array[], char mtype, int line_count);
int check_valid_memdata(int line_count, int stack[], int data[], int heap[], int memsizes[]);


int main(int argc, char * argv[]){
	
	int i = 0, j = 0, char_count = 0, line_count = 0;
	char c;
	char cmdline_input[MAX_NO_OF_ADDRESSES * MAX_INPUT_LINE_LENGTH];
	char line[MAX_INPUT_LINE_LENGTH];
	
	/* check for entered command line parameters */
	if ((argc == 2) && ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0))){
		return print_help(argv[0]);
	}
	
	print_code_promt();
	c = getchar();
	
	
	/* get input from stdin */
	while (c != '\0' && i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) -1){
		if (c == '?'){
			print_help(argv[0]);
			c = getchar();
			print_code_promt();
		} else if (c != '\0' && c != EOF && c != '!'){
			cmdline_input[i++] = c;
			c = getchar();
		} else {
			cmdline_input[i++] = '\0';
		}
	}
	
	/* initital flush */
	flush_buff();
		
	print_back_input(cmdline_input);

	/* Split to single lines and process */
	for (i = 0; i < (MAX_INPUT_LINE_LENGTH * MAX_NO_OF_ADDRESSES) - 1; i++){
		if (cmdline_input[i] != '\n'){
			line[j] = cmdline_input[i];
			char_count++;
			j++;
		
		} else if ((cmdline_input[i] == '\n') && (cmdline_input[i + 1] == '\n')){
			j++;
			
		} else {
			line[j] = '\0';
			process_line(char_count, line_count, line);
			j = 0;
			char_count = 0;
			line_count++;
		}
	}
	
	
	
	/* printf("VERBOSE_DEBUG_OUTPUT: %i", VERBOSE_DEBUG_OUTPUT); */

	if(VERBOSE_DEBUG_OUTPUT){
		/* print back processed mcode */
		print_structured_code(line_count);
	}
	

	/* fill stack, data and heap arrays from mcode[]*/
	
	/* memsizes[i]: 0 = stack, 1 = data, 2 = heap. */
	memsizes[0] = get_mem_size('S', line_count);
	memsizes[1] = get_mem_size('D', line_count);
	memsizes[2] = get_mem_size('H', line_count);
	
	/* stack */
	stack = malloc(memsizes[0] * sizeof(int));
	if (stack == NULL) {
		if(VERBOSE_DEBUG_OUTPUT){
			printf("STACK memory could not be allocated.\n");
		}
		error = 30;
		return 1;
	}
	
	for (i = 0; i < memsizes[0]; i++){
		stack[i] = INT_MIN;
	}
	fill_mem_array(stack, 'S', line_count);
		
	/* data */
	data = malloc(memsizes[1] * sizeof(int));
	if (data == NULL) {
		if(VERBOSE_DEBUG_OUTPUT){
			printf("DATA memory could not be allocated.\n");
		}
		error = 30;
		return 1;
	}
	
	for (i = 0; i < memsizes[1]; i++){
		data[i] = INT_MIN;
	}
	fill_mem_array(data, 'D', line_count);
	
	/* heap */
	heap = malloc(memsizes[2] * sizeof(int));
	if (heap == NULL) {
		if(VERBOSE_DEBUG_OUTPUT){
			printf("HEAP memory could not be allocated.\n");
		}
		error = 30;
		return 1;
	}
	
	for (i = 0; i < memsizes[2]; i++){
		heap[i] = INT_MIN;
	}
	fill_mem_array(heap, 'H', line_count);
	
	
	/* Check for consistent stack, data and heap variables against user input. If non existent query user and add to s,d,h array. */
	
	if (check_valid_memdata(line_count, stack, data, heap, memsizes)){
		if(VERBOSE_DEBUG_OUTPUT){
			printf("\nAll memory data consistent.\n\n");
		}
	} else {
		error = 22;
	}

	if(VERBOSE_DEBUG_OUTPUT){
		/* print back processing of mcode to memory arrays */
		for (i = 0; i < memsizes[2]; i++){
			printf("H%i: %i\n", i + 1, heap[i]);
		}
			
		for (i = 0; i < memsizes[1]; i++){
			printf("D%i: %i\n", i + 1, data[i]);
		}

		for (i = 0; i < memsizes[0]; i++){
			printf("S%i: %i\n", i + 1, stack[i]);
		}
	}
	
	printf("\nEXECUTE:\n\n");
	execute_program();

	free(heap);
	free(stack);
	free(data);

	(error == 0) ? printf("\nExitCode: OK\n") : printf("\nExitCode: ERROR %i\n", error);
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
			
			if ((i == 0) && toupper(line[i]) == 'P'){	
				temp_line.iscommand = 1;
				temp_line.regtype = 'P';
				while(line[j] != 32 && line[j] != 9 && line[j] != 44){	
					temp[j - 1] = line[j];
					j++;
				}
				temp[j - 1] = '\0';
				temp_line.step = sum_array(temp);
				div_count++;
										
			} else if ((i == 0) && toupper(line[i]) == 'D'){	
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
				
			} else if ((i == 0) && toupper(line[i]) == 'S'){	
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
				
					
								
			} else if ((i == 0) && toupper(line[i]) == 'H'){	
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
				if(VERBOSE_DEBUG_OUTPUT){
					printf("\nInvalid programm code in line: %i.\n", line_count + 1);
				}
				error = 10;
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
						if(VERBOSE_DEBUG_OUTPUT){
							printf("\nInvalid command in line: %i\n", line_count + 1);
						}
						error = 13;
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

				if ((toupper(line[j]) == 'P' || toupper(line[j]) == 'D' || toupper(line[j]) == 'S' || toupper(line[j]) == 'H')){
					temp_line.type1 = toupper(line[j]);				
				} else {
					if(VERBOSE_DEBUG_OUTPUT){
						printf("Invalid programm code in line: %i.\n", line_count + 1);
					}
					error = 10;
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
				
				if ((toupper(line[j]) == 'P' || toupper(line[j]) == 'D' || toupper(line[j]) == 'S' || toupper(line[j]) == 'H')){	
					temp_line.type2 = toupper(line[j]);	
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

		
		if (VERBOSE_DEBUG_OUTPUT == 2){
			
			/* DEBUG START: Shows Error in Lines */
			
			printf("Line: %i\t", line_count + 1);
			printf("RegType: %c\t", temp_line.regtype);
			printf("IsC: %i\t", temp_line.iscommand);
			printf("step: %i\t", temp_line.step);
			printf("command: %s\t", temp_line.command);
			printf("type1: %c\t", temp_line.type1);
			printf("val1: %i\t", temp_line.val1);
			printf("type2: %c\t", temp_line.type2);
			printf("val2: %i\n", temp_line.val2);
		}
						
			mcode[line_count] = temp_line;
			return 1;
		} 
	
	}
	return 0;
	
}


int sum_array(char line[]){
	
	int i = 0, sum = 0, factor = 1, vz = 1;
	
	/* find sign */
	if (line[i] == 45){
		vz = -1;
		i++;
	}
	
	/* get number from ascii */
	if (line[i] > 47 && line[i] < 58 && line[i] != '\0'){
		 
		while (line[i] > 47 && line[i] < 58 && line[i] != '\0'){
			sum = sum * factor + ((line[i] - 48));
			if (factor == 1){
				factor = 10;
			}
			i++;
		}
					
	} else {
		return INT_MIN;
	}
	
	return sum * vz;
}


int get_mem_size(char mtype, int line_count){
	
	int i;
	int size = 0, double_check = 0, index_count = 1;
	
	for (i = 0; i < line_count; i++){
		if ((mcode[i].regtype == mtype) && (mcode[i].val1 >= index_count)){
			size++;
			index_count = size;
			
		} else if ((mcode[i].regtype == mtype) && (mcode[i].val1 < index_count)) {
			if(VERBOSE_DEBUG_OUTPUT){
				printf("Error in program code, memory of type %c assigned more than once with same address value!\n\n", mcode[i].regtype);
			}
			error = 12;
			return -1;
			
		}
	} 

	for (i = 0; i < line_count; i++){
		if ((mcode[i].type1 == mtype) && (mcode[i].val1 > double_check)){
				double_check = mcode[i].val1;
			}
	}

	if(VERBOSE_DEBUG_OUTPUT == 2){
		printf("\n%c type memory addresses used in program code: %i\n", mtype, double_check);
		printf("%c type memory addresses defined in file: %i\n", mtype, size);
		if (double_check > size){
			printf("%c type memory addresses used in code but undefined: %i\n", mtype, double_check - size);
		} else if (double_check < size){ 
			printf("%c type memory addresses defined but not used in code: %i\n", mtype, size - double_check);
		}
	}
	
	if (double_check > size){
		size = double_check;
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


int check_valid_memdata(int line_count, int stack[], int data[], int heap[], int memsizes[]){
	
	int i;
		
	for (i = 0; i < line_count; i++){
		if (mcode[i].iscommand){
			
			/* for INIT fill address with 0 */
			
			if(strcmp(mcode[i].command, "INIT") == 0 ){
				if (mcode[i].type1 == 'S'){
					stack[(mcode[i].val1 - 1)] = 0; 
				} else if (mcode[i].type1 == 'D'){
					data[(mcode[i].val1 - 1)] = 0; 
				} else if (mcode[i].type1 == 'H'){
					heap[(mcode[i].val1 - 1)] = 0; 
				} else {
					if(VERBOSE_DEBUG_OUTPUT){
						printf("Invalid memory type. Can not initialize.\n");
					}
					error = 22;
					return 0;
				}
			}
				
				
		}
		
	}
	
	
	
	/* get missing values for stack, data and heap */
	
	for (i = 0; i < memsizes[0]; i++){
		
		do{
			
			if (stack[i] == INT_MIN){
				printf("\nPlease enter Value for S%i:\n\n", i + 1);
				stack[i] = read_buff();
				if (stack[i] == INT_MIN){
					printf("Invalid input\n");
					error = 15;
					return 0;
				}
				
			}
		}
		while (stack[i] == INT_MIN);
		
	}
	
	for (i = 0; i < memsizes[1]; i++){
		
		do{
			
			if (data[i] == INT_MIN){
				printf("\nPlease enter Value for D%i:\n\n", i + 1);
				data[i] = read_buff();
				if (data[i] == INT_MIN){
					printf("Invalid input\n");
					error = 15;
					return 0;
				}
				
			}
		}
		while (data[i] == INT_MIN);
		
	}
	
	for (i = 0; i < memsizes[2]; i++){
		
		do{
			
			if (heap[i] == INT_MIN){
				printf("\nPlease enter Value for H%i:\n\n", i + 1);
				heap[i] = read_buff();
				if (heap[i] == INT_MIN){
					printf("Invalid input\n");
					error = 15;
					return 0;
				}
				
			}
		}
		while (heap[i] == INT_MIN);
		
	}
		
	
	return 1;
}

