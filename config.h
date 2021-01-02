#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/* This is the number of addresses that can be used in the machine program for memory and stack. (S1, S2, S3, D1, D2, etc) */
#define MAX_NO_OF_ADDRESSES 50
/* This is the maximum length that the address can have (e.g. S99 would be valid, S100 would not) */
#define MAX_ADDRESS_SIZE 4
/* This is the maximum content that can be saved under one address */
#define MAX_ADDRESS_CONTENT 10
/* Max length for a line of pseudo machine code */
#define MAX_INPUT_LINE_LENGTH 22
/* Enables verbose debug logging to the console (0: disabled, 1: enabled) */
#define VERBOSE_DEBUG_OUTPUT 0

#endif