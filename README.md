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
* Am Ende soll die Anzahl der benötigten Takte ausgegeben werden.
* Seperater rückgabe block am ende