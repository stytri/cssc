#include <stdio.h>
static void license(void) {
	puts("MIT License");
	puts("");
	puts("Copyright (c) 2025 Tristan Styles");
	puts("");
	puts("Permission is hereby granted, free of charge, to any person obtaining a copy");
	puts("of this software and associated documentation files (the \"Software\"), to deal");
	puts("in the Software without restriction, including without limitation the rights");
	puts("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
	puts("copies of the Software, and to permit persons to whom the Software is");
	puts("furnished to do so, subject to the following conditions:");
	puts("");
	puts("The above copyright notice and this permission notice shall be included in all");
	puts("copies or substantial portions of the Software.");
	puts("");
	puts("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	puts("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	puts("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
	puts("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	puts("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	puts("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
	puts("SOFTWARE.");
}
#ifndef VERSION
#	define VERSION  1.1.1
#endif
//
// Build with https://github.com/stytri/m
//
// ::compile
// :+  $CC $CFLAGS $XFLAGS $SMALL-BINARY
// :+      -o $+^ $"* $"!
//
// ::debug
// :+  $CC $CFLAGS
// :+      -Og -g -DDEBUG_$: -o $^-$+: $"!
// :&  $DBG -tui --args $^-$+: $"*
// :&  $RM $^-$+:
//
// ::-
// :+  $CC $CFLAGS $XFLAGS $SMALL-BINARY
// :+      -o $+: $"* $"!
//
// ::CFLAGS!CFLAGS
// :+      -Wall -Wextra $WINFLAGS $INCLUDE
//
// ::XFLAGS!XFLAGS
// :+      -DNDEBUG=1 -O3 -march=native
//
// ::SMALL-BINARY
// :+      -fmerge-all-constants -ffunction-sections -fdata-sections
// :+      -fno-unwind-tables -fno-asynchronous-unwind-tables
// :+      -Wl,--gc-sections -s
//
// ::windir?WINFLAGS
// :+      -D__USE_MINGW_ANSI_STDIO=1
//
// ::INCLUDE!INCLUDE
// :+      -I ../inc
//
#include <string.h>
static char const *getfilename(char const *cs, int *lenp);
static void usage(char *arg0, FILE *out);
static void version(void) {
#define VERSION__STR(VERSION__STR__version)  #VERSION__STR__version
#define VERSIONTOSTR(VERSIONTOSTR__version)  VERSION__STR(VERSIONTOSTR__version)
	puts("## Version "VERSIONTOSTR(VERSION));
}
static void readme(char *arg0) {
	int         n;
	char const *file = getfilename(arg0, &n);
	printf("# %.*s\n", n, file);
	puts("");
	version();
	puts("");
	puts("## Command Line");
	puts("");
	puts("```");
	usage(arg0, stdout);
	puts("```");
	puts("");
	puts("## Building");
	puts("");
	puts("Uses [HOL](https://github.com/stytri/hol) and [defer](https://github.com/stytri/defer).");
	puts("");
	puts("Compile with [m](https://github.com/stytri/m).");
	puts("");
}

//------------------------------------------------------------------------------

#include <hol/holibc.h>  // https://github.com/stytri/hol
#include <defer.h>       // https://github.com/stytri/defer

//------------------------------------------------------------------------------

#ifdef __MINGW64__
int _dowildcard = -1;
#endif

static struct optget options[] = {
	{  0, "Converts double-quote strings to a sequence of single-quote characters", NULL },
	{  0, "usage: %s [OPTION]... [FILE]...", NULL },
	{  0, "options:",             NULL },
	{  1, "-h, --help",           "display this help and exit" },
	{  2, "    --version",        "display version and exit" },
	{  3, "    --license",        "display license and exit" },
	{  4, "    --readme",         "display readme and exit" },
	{  9, "-o, --output FILE",    "output to FILE" },
	{ 10, "-p, --preprocess",     "treat C style pre-processing directives as comments" },
	{ 11, "-c, --comment TYPE",   "select comment TYPE" },
	{  0, "",                     "C     C style comments" },
	{  0, "",                     "sh    shell style comments" },
	{  0, "",                     "asm   assembler style comments" },
	{  0, "",                     "CHAR  a specified single CHARacter" },
	{ 12, "-s, --seperator TEXT", "character seperator is TEXT" },
	{ 13, "-n, --no-nul",         "do not nul terminate string" },
	{ 14, "-q, --ignore-single-quote", "ignore single quotes" },
	{ 15, "-d, --digit-separator CHAR", "digits in numbers can be separated by CHAR" },
};
static size_t const n_options = (sizeof(options) / sizeof(options[0]));

static void usage(char *arg0, FILE *out) {
	optuse(n_options, options, arg0, out);
}

static inline bool qerror(char const *cs) {
	perror(cs);
	return false;
}

int
main(
	int    argc,
	char **argv
) {
	char const *infile = NULL;
	char const *outfile = NULL;
	char const *separator = ",";
	char        comment = '/';
	bool        preprocess = true;
	bool        nulterminate = true;
	bool        singlequote = true;
	char        digitseparator = 0;

	int argi = 1;
	while((argi < argc) && (*argv[argi] == '-')) {
		char const *args = argv[argi++];
		char const *argp = NULL;
		do {
			int argn   = argc - argi;
			int params = 0;
			switch(optget(n_options - 2, options + 2, &argp, args, argn, &params)) {
			case 1:
				usage(argv[0], stdout);
				return 0;
			case 2:
				version();
				return 0;
			case 3:
				license();
				return 0;
			case 4:
				readme(argv[0]);
				return 0;
			case 9:
				outfile = argv[argi];
				break;
			case 10:
				preprocess = true;
				break;
			case 11:
				if(strcmp(argv[argi], "c") == 0) {
					comment = '/';
				} else if(strcmp(argv[argi], "sh") == 0) {
					comment = '#';
				} else if(strcmp(argv[argi], "asm") == 0) {
					comment = ';';
				} else {
					comment = *argv[argi];
				}
				break;
			case 12:
				separator = argv[argi];
				break;
			case 13:
				nulterminate = false;
				break;
			case 14:
				singlequote = false;
				break;
			case 15:
				digitseparator = *argv[argi];
				break;
			default:
				errorf("invalid option: %s", args);
				usage(argv[0], stderr);
				return EXIT_FAILURE;
			}
			argi += params;
		} while(argp)
			;
	}

	bool failed = false;
	DEFER(FILE *out = outfile ? fopen(outfile, "w") : stdout,
		!(failed = !out) || qerror(outfile),
		(out != stdout) ? fclose(out) : 0
	) do {
		if(argi < argc) {
			infile = argv[argi++];
		}
		DEFER(FILE *in = infile ? fopen(infile, "r") : stdin,
			!(failed = !in) || qerror(infile),
			(in != stdin) ? fclose(in) : 0
		) {
			bool   sol = preprocess;
			bool   character = false;
			size_t quoted = 0;
			for(int c; (c = fgetc(in)) != EOF; ) {
				if(sol) {
					sol = !isgraph(c);
					if(c == '#') {
						do {
							fputc(c, out);
							c = fgetc(in);
							if(c == '\\') {
								fputc(c, out);
								c = fgetc(in);
								if(c == '\n') {
									fputc(c, out);
									c = fgetc(in);
								}
							}
						} while((c != '\n') && (c != EOF))
							;
						if(c == EOF) {
							break;
						}
						fputc(c, out);
						sol = preprocess;
						continue;
					}
				}
				if(character || quoted) {
					if(character) {
						if(c == '\'') {
							character = false;
							fputc(c, out);
							continue;
						}
					} else {
						if(c == '"') {
							if(nulterminate) {
								if(quoted++ > 1) {
									fputs(separator, out);
								}
								fputs("'\\0'", out);
							}
							quoted = 0;
							continue;
						}
						if(quoted++ > 1) {
							fputs(separator, out);
						}
						if(quoted) {
							fputc('\'', out);
						}
					}
					switch(c) {
					case '\\':
						fputc(c, out);
						c = fgetc(in);
						if(('0' <= c) && (c <= '7')) {
							do {
								fputc(c, out);
								c = fgetc(in);
							} while(('0' <= c) && (c <= '7'))
								;
							ungetc(c, in);
							break;;
						} else switch(c) {
						case 'x': case 'u': case 'U':
							fputc(c, out);
							c = fgetc(in);
							while(isxdigit(c)) {
								fputc(c, out);
								c = fgetc(in);
							}
							ungetc(c, in);
							break;
						default:
							fputc(c, out);
							break;
						}
						break;
					case '\'':
						if(character) {
							fputc('\\', out);
						}
						break;
					case '\t': fputc('\\', out); fputc('t', out); break;
					case '\n': fputc('\\', out); fputc('n', out); break;
					case '\r': fputc('\\', out); fputc('r', out); break;
					default  :
						if(!isprint(c)) {
							fputc('\\', out);
							fputc('0', out);
							if(c > 077) {
								fputc("01234567"[(c >> 6) & 0x7], out);
							}
							if(c > 007) {
								fputc("01234567"[(c >> 3) & 0x7], out);
							}
							fputc("01234567"[c& 0x7], out);
							fputc('\'', out);
							break;
						}
						fputc(c, out);
						break;
					}
					if(quoted) {
						fputc('\'', out);
					}
					continue;
				}
				if(c == '"') {
					quoted = 1;
					continue;
				}
				if(c == '\'') {
					character = singlequote;
					fputc(c, out);
					continue;
				}
				if(c == comment) {
					if(c != '/') {
						do {
							fputc(c, out);
							c = fgetc(in);
						} while((c != '\n') && (c != EOF))
							;
					} else {
						fputc(c, out);
						c = fgetc(in);
						if(c == '/') {
							do {
								fputc(c, out);
								c = fgetc(in);
							} while((c != '\n') && (c != EOF))
								;
						} else if(c == '*') {
							do {
								fputc(c, out);
								c = fgetc(in);
								if(c == '*') {
									fputc(c, out);
									c = fgetc(in);
									if(c == '/') {
										break;
									}
								}
							} while(c != EOF)
								;
						}
					}
					if(c == EOF) {
						break;
					}
				}
				if(digitseparator && isdigit(c)) {
					int (*is_digit)(int) = isdigit;
					if(c == '0') {
						do {
							fputc(c, out);
							c = fgetc(in);
						} while(c == '0')
							;
						switch(tolower(c)) {
						case 'x':
							is_digit = isxdigit;
							[[fallthrough]];
						case 'b':
							c = fgetc(in);
							break;
						default :
							break;
						}
					} else {
						fputc(c, out);
						c = fgetc(in);
					}
					while(is_digit(c) || (c == digitseparator)) {
						fputc(c, out);
						c = fgetc(in);
					}
					if(c == EOF) {
						break;
					}
					ungetc(c, in);
					continue;
				}
				fputc(c, out);
				if(c == '\n') {
					sol = preprocess;
				}
			}
		}
	} while(!failed && (argi < argc))
		;
	return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
