#include "simulator.h"
#include "types.h"
#include "config.h"

int execute_program_line();
int init(char register_type, int register_number);
int add(char register_type_1, int register_number_1, char register_type_2, int register_number_2);
int sub(char register_type_1, int register_number_1, char register_type_2, int register_number_2);
int decrement(char register_type, int register_number);
int cond_decrement(char decr_register_type, int decr_register_number, char cond_register_type, int cond_register_number);
int increment(char register_type, int register_number);
int cond_increment(char incr_register_type, int incr_register_number, char cond_register_type, int cond_register_number);
int jump(char register_type, int register_number);
int cond_jump(char jump_register_type, int jump_register_number, char cond_register_type, int cond_register_number);
int return_n(char return_register_type, int return_register_number);
int cond_return_n(char return_register_type, int return_register_number, char cond_register_type, int cond_register_number);
int write_value(char register_type, int register_number, int value);
int get_value(char register_type, int register_number);

extern struct mcode_line mcode[MAX_NO_OF_ADDRESSES];

/* 	Starts and controls the excecution of the program
		Returns 0 when the complete program excecuted correctly.
		Returns 1 when there was an error during the excecution of a code line.
		Returns 2 when no program lines where found or there is no register P1.
*/
int execute_program()
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
			int line_status = execute_program_line(current_code_line);
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
int execute_program_line(struct mcode_line* code_line)
{
	printf("Fuehre Zeile %c%i aus\n", (*code_line).regtype, (*code_line).step);

	if(strcmp((*code_line).command, "INIT") == 0 )
	{
		int status = init((*code_line).type1, (*code_line).val1);
		printf("Executed INIT\n");
	}
	else if(strcmp((*code_line).command, "ADD") == 0 )
	{
		int status = add((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
		printf("Executed ADD\n");
	}
	else if(strcmp((*code_line).command, "SUB") == 0 )
	{
		int status = sub((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
		printf("Executed SUB\n");
	}
	else if(strcmp((*code_line).command, "DECREMENT") == 0 )
	{
		int status = decrement((*code_line).type1, (*code_line).val1);
		printf("Executed DECREMENT\n");
	}
	else if(strcmp((*code_line).command, "DECREMENT0") == 0 )
	{
		int status = cond_decrement((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
		printf("Executed DECREMENT0\n");
	}
	else if(strcmp((*code_line).command, "INCREMENT") == 0 )
	{
		int status = increment((*code_line).type1, (*code_line).val1);
		printf("Executed INCREMENT\n");
	}
	else if(strcmp((*code_line).command, "INCREMENT0") == 0 )
	{
		int status = cond_increment((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
		printf("Executed INCREMENT0\n");
	}
	else if(strcmp((*code_line).command, "JUMP") == 0 )
	{
		int status = jump((*code_line).type1, (*code_line).val1);
		printf("Executed JUMP\n");
	}
	else if(strcmp((*code_line).command, "JUMP0") == 0 )
	{
		int status = cond_jump((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
		printf("Executed JUMP0\n");
	}
	else if(strcmp((*code_line).command, "RETURN") == 0 )
	{
		int status = return_n((*code_line).type1, (*code_line).val1);
		printf("Executed RETURN\n");
	}
	else if(strcmp((*code_line).command, "RETURN0") == 0 )
	{
		int status = cond_return_n((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
		printf("Executed RETURN0\n");
	}
	else
	{
		printf("%s ist kein valider Befehl.\n", (*code_line).command);
		return 1;
	}
	
	return 0;
}

int init(char register_type, int register_number)
{
	return 0;
}

int add(char register_type_1, int register_number_1, char register_type_2, int register_number_2)
{
	return 0;
}

int sub(char register_type_1, int register_number_1, char register_type_2, int register_number_2)
{
	return 0;
}

int decrement(char register_type, int register_number)
{
	return 0;
}

int cond_decrement(char decr_register_type, int decr_register_number, char cond_register_type, int cond_register_number)
{
	return 0;
}

int increment(char register_type, int register_number)
{
	return 0;
}

int cond_increment(char incr_register_type, int incr_register_number, char cond_register_type, int cond_register_number)
{
	return 0;
}

int jump(char register_type, int register_number)
{
	return 0;
}

int cond_jump(char jump_register_type, int jump_register_number, char cond_register_type, int cond_register_number)
{
	return 0;
}

int return_n(char return_register_type, int return_register_number)
{
	return 0;
}

int cond_return_n(char return_register_type, int return_register_number, char cond_register_type, int cond_register_number)
{
	return 0;
}


int write_value(char register_type, int register_number, int value)
{
	return 0;
}

int get_value(char register_type, int register_number)
{
	return 0;
}