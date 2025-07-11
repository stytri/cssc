# cssc

## Version 1.1.0

## Command Line

```
Converts double-quote strings to a sequence of single-quote characters
usage: cssc [OPTION]... [FILE]...
options:
  -h, --help            display this help and exit
      --version         display version and exit
      --license         display license and exit
      --readme          display readme and exit
  -o, --output FILE     output to FILE
  -p, --preprocess      treat C style pre-processing directives as comments
  -c, --comment TYPE    select comment TYPE
                        C     C style comments
                        sh    shell style comments
                        asm   assembler style comments
  -s, --seperator TEXT  character seperator is TEXT
  -n, --no-nul          do not nul terminate string
```

## Building

Uses [HOL](https://github.com/stytri/hol) and [defer](https://github.com/stytri/defer).

Compile with [m](https://github.com/stytri/m).

