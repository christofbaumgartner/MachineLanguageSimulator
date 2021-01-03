#include "simulator.h"
#include "global.h"
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

struct mcode_line mcode[MAX_NO_OF_ADDRESSES];
int *stack, *data, *heap;
int program_pointer = 1;
int clock_number = 0;

/* 	Starts and controls the excecution of the program
		Returns 0 when the complete program excecuted correctly.
		Returns 1 when there was an error during the excecution of a code line.
		Returns 2 when no program lines where found or there is no register P1.
*/
int execute_program()
{
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
			if(((*current_code_line).regtype == 'P') && ((*current_code_line).step == program_pointer ))
			{
				program_line_found = 1;
				program_pointer++;
				break;
			}
			program_line_found = 0;
			current_code_line++;
		}
		/* Program line was found */
		if(program_line_found == 1)
		{
			int line_status;
			printf("Executing register: %c%i\n", (*current_code_line).regtype, (*current_code_line).step );
			line_status = execute_program_line(current_code_line);
			clock_number++;
			if(line_status != 0)
			{
				printf("Error while processing line: %i\n", program_pointer - 1);
				return 1;
			}
			continue;
		}
		/* no P1 was found in the program */
		else if(program_line_found == -1)
		{
			printf("No program code or no register P1\n");
			return 2;
		}
		/* No P Register with the current program_pointer was not found -> End Program */
		else
		{
			printf("Program termination line: %i\n", program_pointer - 1);
			printf("Required cycles: %i\n", clock_number);
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
	if(strcmp((*code_line).command, "INIT") == 0 )
	{
		return init((*code_line).type1, (*code_line).val1);
	}
	else if(strcmp((*code_line).command, "ADD") == 0 )
	{
		return add((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
	}
	else if(strcmp((*code_line).command, "SUB") == 0 )
	{
		return sub((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
	}
	else if(strcmp((*code_line).command, "DECREMENT") == 0 )
	{
		return decrement((*code_line).type1, (*code_line).val1);
	}
	else if(strcmp((*code_line).command, "DECREMENT0") == 0 )
	{
		return cond_decrement((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
	}
	else if(strcmp((*code_line).command, "INCREMENT") == 0 )
	{
		return increment((*code_line).type1, (*code_line).val1);
	}
	else if(strcmp((*code_line).command, "INCREMENT0") == 0 )
	{
		return cond_increment((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
	}
	else if(strcmp((*code_line).command, "JUMP") == 0 )
	{
		return jump((*code_line).type1, (*code_line).val1);
	}
	else if(strcmp((*code_line).command, "JUMP0") == 0 )
	{
		return cond_jump((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
	}
	else if(strcmp((*code_line).command, "RETURN") == 0 )
	{
		return return_n((*code_line).type1, (*code_line).val1);
	}
	else if(strcmp((*code_line).command, "RETURN0") == 0 )
	{
		return cond_return_n((*code_line).type1, (*code_line).val1, (*code_line).type2, (*code_line).val2);
	}
	else
	{
		printf("%s is not a valid command.\n", (*code_line).command);
		return 1;
	}
	
	return 0;
}

int init(char register_type, int register_number)
{
	int result = write_value(register_type, register_number, 0);
	printf("\tCOMMAND: INIT %c%i\n", register_type, register_number);
	printf("\tREGISTER: %c%i: %i\n", register_type, register_number, 0);
	return result;
}

int add(char register_type_1, int register_number_1, char register_type_2, int register_number_2)
{
	int value = get_value(register_type_1, register_number_1);
	int value2 = get_value(register_type_2, register_number_2);
	int result = write_value(register_type_1, register_number_1, value + value2);
	printf("\tCOMMAND: ADD %c%i,%c%i\n", register_type_1, register_number_1, register_type_2, register_number_2);
	printf("\tREGISTER: %c%i: %i, %c%i: %i\n", register_type_1, register_number_1, value + value2, register_type_2, register_number_2, value2 );
	return result;
}

int sub(char register_type_1, int register_number_1, char register_type_2, int register_number_2)
{
	int value = get_value(register_type_1, register_number_1);
	int value2 = get_value(register_type_2, register_number_2);
	int result = write_value(register_type_1, register_number_1, value - value2);
	printf("\tCOMMAND: SUB %c%i,%c%i\n", register_type_1, register_number_1, register_type_2, register_number_2);
	printf("\tREGISTER: %c%i: %i, %c%i: %i\n", register_type_1, register_number_1, value - value2, register_type_2, register_number_2, value2 );
	return result;
}

int decrement(char register_type, int register_number)
{
	int value = get_value(register_type, register_number);
	int result = write_value(register_type, register_number, --value);
	printf("\tCOMMAND: DECREMENT %c%i\n", register_type, register_number);
	printf("\tREGISTER: %c%i: %i\n", register_type, register_number, value);
	return result;
}

int cond_decrement(char decr_register_type, int decr_register_number, char cond_register_type, int cond_register_number)
{
	int value = get_value(decr_register_type, decr_register_number);
	int cond_value = get_value(cond_register_type, cond_register_number);
	printf("\tCOMMAND: DECREMENT0 %c%i, %c%i\n", decr_register_type, decr_register_number, cond_register_type, cond_register_number);
	if(cond_value == 0)
	{
		int result = write_value(decr_register_type, decr_register_number, --value);
		printf("\tCONDITION in %c%i is %i. Value decreased\n", cond_register_type, cond_register_number, cond_value);
		printf("\tREGISTER: %c%i: %i, %c%i: %i\n", decr_register_type, decr_register_number, value, cond_register_type, cond_register_number, cond_value );
		return result;
	}
	printf("\tCONDITION in %c%i is %i. Value not decreased\n", cond_register_type, cond_register_number, cond_value);
	printf("\tREGISTER: %c%i: %i, %c%i: %i\n", decr_register_type, decr_register_number, value, cond_register_type, cond_register_number, cond_value );
	return 0;
}

int increment(char register_type, int register_number)
{
	int value = get_value(register_type, register_number);
	int result = write_value(register_type, register_number, ++value);
	printf("\tCOMMAND: INCREMENT %c%i\n", register_type, register_number);
	printf("\tREGISTER: %c%i: %i\n", register_type, register_number, value);
	return result;
}

int cond_increment(char incr_register_type, int incr_register_number, char cond_register_type, int cond_register_number)
{
	int value = get_value(incr_register_type, incr_register_number);
	int cond_value = get_value(cond_register_type, cond_register_number);
	printf("\tCOMMAND: DECREMENT0 %c%i, %c%i\n", incr_register_type, incr_register_number, cond_register_type, cond_register_number);
	if(cond_value == 0)
	{
		int result = write_value(incr_register_type, incr_register_number, ++value);
		printf("\tCONDITION in %c%i is %i. Value increased\n", cond_register_type, cond_register_number, cond_value);
		printf("\tREGISTER: %c%i: %i, %c%i: %i\n", incr_register_type, incr_register_number, result, cond_register_type, cond_register_number, cond_value );
		return result;
	}
	printf("\tCONDITION in %c%i is %i. Value not increased\n", cond_register_type, cond_register_number, cond_value);
	printf("\tREGISTER: %c%i: %i, %c%i: %i\n", incr_register_type, incr_register_number, value, cond_register_type, cond_register_number, cond_value );
	return 0;
}

int jump(char register_type, int register_number)
{
	printf("\tCOMMAND: JUMP %c%i\n", register_type, register_number);
	if(register_type == 'P')
	{
		program_pointer = register_number;
		return 0;
	}
	return 1;
}

int cond_jump(char jump_register_type, int jump_register_number, char cond_register_type, int cond_register_number)
{
	int cond_value = get_value(cond_register_type, cond_register_number);
	printf("\tCOMMAND: JUMP0 %c%i, %c%i\n", jump_register_type, jump_register_number, cond_register_type, cond_register_number);
	if(cond_value == 0 && jump_register_type == 'P')
	{
		printf("\tCONDITION in %c%i is %i. Jump to %c%i\n", cond_register_type, cond_register_number, cond_value, jump_register_type, jump_register_number);
		program_pointer = jump_register_number;
		return 0;
	}
	else if(cond_value != 0)
	{
		printf("\tCONDITION in %c%i is %i. No jump\n", cond_register_type, cond_register_number, cond_value);
		return 0;
	}
	return 1;
}

int return_n(char return_register_type, int return_register_number)
{
	int value = get_value(return_register_type, return_register_number);
	printf("\tCOMMAND: RETURN %c%i\n", return_register_type, return_register_number);
	printf("\tRETURN: %c%i: %i\n", return_register_type, return_register_number, value);
	return 0;
}

int cond_return_n(char return_register_type, int return_register_number, char cond_register_type, int cond_register_number)
{
	int cond_value = get_value(cond_register_type, cond_register_number);
	int value = get_value(return_register_type, return_register_number);
	printf("\tCOMMAND: RETURN0 %c%i, %c%i\n", return_register_type, return_register_number, cond_register_type, cond_register_number);
	if(cond_value == 0)
	{
		printf("\tCONDITION in %c%i is %i.\n", cond_register_type, cond_register_number, cond_value);
		printf("\tRETURN: %c%i: %i\n", return_register_type, return_register_number, value);
		return 0;
	}
	printf("\tCONDITION in %c%i is %i. No return.\n", cond_register_type, cond_register_number, cond_value);
	return 1;
}

/* Internal function to write values to a register 
	Returns 0 when the value was written
	Returns 1 when no value was written
	*/
int write_value(char register_type, int register_number, int value)
{
	if(register_type == 'D')
	{
		data[register_number - 1] = value;
		return 0;
	}
	else if(register_type == 'S')
	{
		stack[register_number - 1] = value;
		return 0;
	}
	else if(register_type == 'H')
	{
		heap[register_number - 1] = value;
		return 0;
	}

	return 1;
}

/* Internal function to get values from a register
	Returns the value
	Returns 0 when the register is not valid
	*/
int get_value(char register_type, int register_number)
{
	if(register_type == 'D')
	{
		return data[register_number - 1];
	}
	else if(register_type == 'S')
	{
		return stack[register_number - 1];
	}
	else if(register_type == 'H')
	{
		return heap[register_number - 1];
	}
	return 0;
}