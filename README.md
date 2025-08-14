# cssc

## Version 1.2.0

## Command Line

```
Converts double-quote strings to a sequence of single-quote characters
usage: cssc [OPTION]... [FILE]...
options:
  -h, --help                  display this help and exit
      --version               display version and exit
      --license               display license and exit
      --readme                display readme and exit
  -o, --output FILE           output to FILE
  -p, --preprocess            treat C style pre-processing directives as comments
  -c, --comment TYPE          select comment TYPE
                              C     C style comments
                              sh    shell style comments
                              asm   assembler style comments
                              CHAR  a specified single CHARacter
  -s, --seperator TEXT        character seperator is TEXT
  -n, --no-nul                do not nul terminate string
  -q, --ignore-single-quote   ignore single quotes
  -d, --digit-separator CHAR  digits in numbers can be separated by CHAR
CHAR:                         Either a single punctuation character, or, named character (see README)
```

## Named Characters
-	`&amp;` yields character `&`
-	`&apos;` yields character `'`
-	`&ast;` yields character `*`
-	`&bsol;` yields character `\`
-	`&colon;` yields character `:`
-	`&comma;` yields character `,`
-	`&commat;` yields character `@`
-	`&dollar;` yields character `&`
-	`&equals;` yields character `=`
-	`&excl;` yields character `!`
-	`&hat;` yields character `^`
-	`&lbrace;` yields character `{`
-	`&lbrack;` yields character `[`
-	`&lowbar;` yields character `_`
-	`&lpar;` yields character `(`
-	`&lt;` yields character `<`
-	`&gt;` yields character `>`
-	`&minus;` yields character `-`
-	`&num;` yields character `#`
-	`&percnt;` yields character `%`
-	`&period;` yields character `.`
-	`&plus;` yields character `+`
-	`&quest;` yields character `?`
-	`&quot;` yields character `"`
-	`&rbrace;` yields character `}`
-	`&rbrack;` yields character `]`
-	`&rpar;` yields character `)`
-	`&semi;` yields character `;`
-	`&sol;` yields character `/`
-	`&tilde;` yields character `~`
-	`&vert;` yields character `|`

## Building

Uses [HOL](https://github.com/stytri/hol) and [defer](https://github.com/stytri/defer).

Compile with [m](https://github.com/stytri/m).

