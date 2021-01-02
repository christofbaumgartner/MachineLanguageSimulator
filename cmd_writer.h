#ifndef CMD_WRITER_INCLUDED
#define CMD_WRITER_INCLUDED

#include <stdio.h>
#include "global.h"
#include "config.h"

int print_help(char *filename);
int print_code_promt();
int print_back_input();
int print_structured_code(int line_count);

#endif