# Machine Language Simulator

 This is a program to simulate the execution of machine code.
 
 User preferences can be adjusted in the file config.h before compilation.
 There are setting for max number of lines, max address size, max address content, line length and debug verbosity.
 
 Compile and run 'program.exe' (or your equivalent) and type in your pseudo machine code. When done type '!' on a new line. 
 Alternativley you can pass a file like this: program.exe < yourcode.mc
  
 Valid commands are:

 INIT           A       Store value 0 at address A.
 ADD            A,B     Add to memory address A the value of memory address B.
 SUB            A,B     Substract from memory address A the value of memory address B.
 DECREMENT      A       Decrease the value of memory address A by 1.
 DECREMENT0     A,B     Decrease the value of memory address A by 1 in case value of memory address B equals 0.
 INCREMENT      A       Increase the value of memory address A by 1.
 INCREMENT0     A,B     Increase the value of memory address A by 1 in case value of memory address B equals 0.
 JUMP           A       Jump to program pointer A.
 JUMP0          A,B     Jump to program pointer A in case value of memory address B equals 0.
 RETURN         A       Return value of memory address A.
 RETURN0        A,B     Return value of memory address A in case value of memory address B equals 0.

 PROGRAM step has the format Px while 'x' is the line number.
 DATA address has the format Dx while 'x' is the memory address.
 STACK address has the format Sx while 'x' is the memory address.
 HEAP address has the format Hx while 'x' is the memory address.

 Addresses of successive memory cells are numbered consecutively. Do not leave any gaps.
 
 Valid values for S,D and H are from INT_MIN+1 to INT_MAX.

 Separation either by 'Space', 'Tab' or ','.
 
 Example code:

 P1     INIT            S2
 P2     INCREMENT       S2
 P3     JUMP0           P7,S1
 P4     ADD             S2,S2
 P5     DECREMENT       S1
 P6     JUMP            P3
 P7     RETURN          S2
 S1     2
 S2
 !
 

## Compile

```
gcc main.c simulator.c cmd_writer.c global.c -ansi -pedantic -Wall -Wextra -o program.exe
```

## Compile with make

```
make
```

## Run

```
program.exe
```

## Known issues

* (SOLVED) Limited support for negative numbers
* Mixed register value that starts with digit and contains letters is only read until first letter, no error returned. ie. S1 3234r54 => S1 3234
* (SOLVED) Input buffer is not flushed.
* (SOLVED except for piped iput) User query for missing or uninitialized parameters (i.e. missing value for S1 and no INIT S1).
* User query does not work for piped input, i.e. program.exe < examplecode.mc. Therefore missing or wrong parameters will result in error code.

* Combined summary block of all RETURNS or ERRORS at the end. Possible Errors:
  * 010 - Error Parsing: Unkown Parser Error
  * 011 - Error Parsing: Invalid Register
  * 012 - Error Parsing: Register address out of bounds
  * 013 - Error Parsing: Invalid Command
  * 014 - User Input: Buffer Error
  * 015 - User Input: Invalid input.
  * 021 - Error Execution: Program does not terminate
  * 022 - Error Execution: Register not initialized
  * 023 - Error Execution: Value out of bounds
  * 020 - Error Execution: Unkown Execution Error
  * 030 - System Error: Memory Allocation
  