# Machine Language Simulator

## Compile

```
gcc main.c simulator.c cmd_writer.c -ansi -pedantic -Wall -Wextra -o program.exe
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

* Limited support for negative numbers
* Input buffer is not flushed
* Wenn das Maschinenprogramm Parameter (z.B. in S1) erwartet, soll der Benutzer diese eingeben
können.
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