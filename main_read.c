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

struct mcode_line{
		char regtype;
		int iscommand;
		int step;
		char command[MAX_ADDRESS_CONTENT]; 
		char type1;
		int val1;
		char type2;
		int val2;
	};
	
struct mcode_line mcode[MAX_NO_OF_ADDRESSES];


int process_line(int char_count, int line_count, char line[]);
int sum_array(char sline[]);
int excecute_program();
int excecute_program_line();


int main(int argc, char * argv[]){
	
	
	int i = 0, j = 0, char_count = 0, file_check = 1, line_count = 0, error = 0;
	char c;
	char cmdline_input[MAX_NO_OF_ADDRESSES * MAX_INPUT_LINE_LENGTH];
	char line[MAX_INPUT_LINE_LENGTH];
		
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

	printf("\nAUSFUEHRUNG:\n\n");

	excecute_program();
	
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

/* 	Starts and controls the excecution of the program
		Returns 0 when the complete program excecuted correctly.
		Returns 1 when there was an error during the excecution of a code line.
		Returns 2 when no program lines where found or there is no register P1.
*/
int excecute_program()
{
	int program_pointer = 1;
	int program_line_found = -1; 

	/* This Loop is repeated as long as program lines are found */
	while(1)
	{
		struct mcode_line* current_code_line = mcode;
		struct mcode_line* end_mcode_ptr = mcode + sizeof(mcode)/sizeof(mcode[0]);

		/* Search the machine code for the program pointer */
		while ( current_code_line < end_mcode_ptr )
		{
			/* Search for a Register with type P and the number of the program_pointer */
			if(((*current_code_line).regtype == 'P' || (*current_code_line).regtype == 'p') && ((*current_code_line).step == program_pointer ))
			{
				program_line_found = 1;
				printf("Code Zeile gefunden: %i\n", program_pointer);
				program_pointer++;
				break;
			}
			program_line_found = 0;
			current_code_line++;
		}
		/* Program line was found */
		if(program_line_found == 1)
		{
			int line_status = excecute_program_line(current_code_line);
			if(line_status != 0)
			{
				printf("Fehler beim ausführen der Zeile: %i\n", program_pointer);
				return 1;
			}
			continue;
		}
		/* no P1 was found in the program */
		else if(program_line_found == -1)
		{
			printf("Keinen Programmcode gefunden bzw. kein Register P1\n");
			return 2;
		}
		/* No P Register with the current program_pointer was not found -> End Program */
		else
		{
			printf("Programm beendeet bei Zeile: %i\n", program_pointer - 1);
			break;
		}
	}
	
	return 0;
}

/* 	Excecutes a specific program line
		Returns 0 when the excecution was successful
		Returns 1 when there was an error during the excecution
*/
int excecute_program_line(struct mcode_line* code_line)
{
	printf("Fuehre Zeile %c%i aus\n", (*code_line).regtype, (*code_line).step);

	if(strcmp((*code_line).command, "INIT") == 0 )
	{
		printf("Excecuted INIT\n");
	}
	else if(strcmp((*code_line).command, "ADD") == 0 )
	{
		printf("Excecuted ADD\n");
	}
	else if(strcmp((*code_line).command, "SUB") == 0 )
	{
		printf("Excecuted SUB\n");
	}
	else if(strcmp((*code_line).command, "DECREMENT") == 0 )
	{
		printf("Excecuted DECREMENT\n");
	}
	else if(strcmp((*code_line).command, "DECREMENT0") == 0 )
	{
		printf("Excecuted DECREMENT0\n");
	}
	else if(strcmp((*code_line).command, "INCREMENT") == 0 )
	{
		printf("Excecuted INCREMENT\n");
	}
	else if(strcmp((*code_line).command, "INCREMENT0") == 0 )
	{
		printf("Excecuted INCREMENT0\n");
	}
	else if(strcmp((*code_line).command, "JUMP") == 0 )
	{
		printf("Excecuted JUMP\n");
	}
	else if(strcmp((*code_line).command, "JUMP0") == 0 )
	{
		printf("Excecuted JUMP0\n");
	}
	else if(strcmp((*code_line).command, "RETURN") == 0 )
	{
		printf("Excecuted RETURN\n");
	}
	else if(strcmp((*code_line).command, "RETURN0") == 0 )
	{
		printf("Excecuted RETURN0\n");
	}
	else
	{
		printf("%s ist kein valider Befehl.\n", (*code_line).command);
		return 1;
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