// main.c
// -------
// main program of the len mtutil
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include "../globals.h"
#include <limits.h>

// work with a 256 byte buffer which should be good enough
#define FILE_BUFFER_SIZE 256
// 4kib of input should be sufficient
#define STDIN_MAX_LINE_LENGTH 4096

// arg options, these are the defaults
int  arg_useFile     = 0;        // -f  | --use-file
char arg_separator   = ' ';      // -s  | --separator
long arg_limit       = LONG_MAX; // -l  | --limit
int  arg_noSeparator = 0;        // -ns | --no-separator
int  arg_showHelp    = 0;        // -?  | --help

// lenStr
// desc:
// 	calculates the length of the specified str and puts the value in count
// args:
// 	char *str   - the string, duh
// 	long *count - pointer to the count, duh (part ii)
// 	long limit  - string length limit
void lenStr(char *str, long *count, long limit) {
#ifdef DEBUG
	printf("DEBUG: entered lenStr with count=%d, limit=%d\n", *count, limit);
#endif
	if (arg_noSeparator) {
#ifdef DEBUG
		printf("DEBUG: -ns specified\n");
#endif
		(*count) += strnlen(str, limit);
	}
	else {
		size_t len = strnlen(str, limit);
#ifdef DEBUG
		printf("DEBUG: -ns not specified (btw len=%d)\n", len);
#endif
		for (long i = 0; i < len; i++)
			if (str[i] == arg_separator) (*count)++;
	}
#ifdef DEBUG
	printf("DEBUG: exiting lenStr with count=%d\n", *count);
#endif
}

// lenFile
// desc:
// 	same as lenStr, but with a file
// args:
// 	char *flname - file name
// 	long *count  - ditto
// returns:
// 	exit code for the program
int lenFile(char *flname, long *count) {
#ifdef DEBUG
	printf("DEBUG: entered lenFile with flname='%s'\n", flname);
#endif
	FILE *fl = fopen((const char *)flname, "r");
	if (!fl) {
		printf("failed to open file \"%s\"\n", flname);
		return 1;
	}

	char buffer[FILE_BUFFER_SIZE] = "";
	while (!feof(fl)) {
		memset(buffer, 0, FILE_BUFFER_SIZE);
		fread(buffer, FILE_BUFFER_SIZE, 1, fl);
#ifdef DEBUG
		printf("DEBUG: read '%s'\n", buffer);
#endif
		lenStr(buffer, count, FILE_BUFFER_SIZE);
	}
	fclose(fl);
	printf("%ld\n", (*count));
	return 0;
}

int main(int argc, char **argv) {
	// first we process the args
	int isOptionArg[argc];
	for (int i = 1; i < argc; i++) {
		// skip non-options
		isOptionArg[i] = 0;
		if (argv[i][0] != '-') continue;
		else isOptionArg[i] = 1;
		
		// pretend first argument is file name and not the string to process
		if (orCmpS(argv[i], "-f", "--use-file"))
			arg_useFile = 1;
		// use different separator than space
		else if (orCmpS(argv[i], "-s", "--separator"))
			arg_separator = argv[++i][0];
		// set limit and exit with err code 1 if count exceeds limit
		else if (orCmpS(argv[i], "-l", "--limit"))
			sscanf(argv[++i], "%ld", &arg_limit);
		// disallow separator and just count the character count
		else if (orCmpS(argv[i], "-ns", "--no-separator"))
			arg_noSeparator = 1;
		// show help and exit
		else if (orCmpS(argv[i], "-?", "--help"))
			arg_showHelp = 1;
		// cancel option scanning
		else if (!strncmp(argv[i], "--",
			maxsize((size_t)2, sizeof argv[i]))
		) {
			i++;
			break;
		}
	}
	long count = 1;

	if (arg_showHelp) {
		printf(
			"%s [options...] <args...>\n"
			"%s -f [options...] <file name>\n"
			"gets the length of the specified string(s)/file\n"
			"----\n"
			"options:\n"
			"  -f  | --use-file      makes first argument be the name of a file to read\n"
			"  -s  | --separator     sets a different separator from ' '\n"
			"  -l  | --limit         sets a limit to exit with error code 1 after reaching it\n"
			"  -?  | --help          shows this help\n"
			"  -ns | --no-separator  counts just characters; ignores -s\n"
			"----\n"
			"written by Matto58 and licensed under the MIT license\n"
			"report bugs/suggestions at https://github.com/BrilliantOS/mtutils/issues\n"
			"(english or czech)\n", argv[0], argv[0]);
		return 0;
	}

	// check if we have any non-option args
	int anyNormalArgs = 0;
	for (int i = 1; i < argc; i++) {
		if (!isOptionArg[i]) {
			anyNormalArgs = 1;
			break;
		}
	}

#ifdef DEBUG
	printf("DEBUG: arg parse finished, argc=%d, anyNormalArgs=%d\n", argc, anyNormalArgs);
#endif

	if (arg_noSeparator) count--;
	if (!anyNormalArgs) {
		char input[STDIN_MAX_LINE_LENGTH];
		do {
			fgets(input, STDIN_MAX_LINE_LENGTH, stdin);
#ifdef DEBUG
			printf("DEBUG: inputted '%s'\n", input);
			printf("DEBUG: last char is %d\n", input[strlen(input)-2]);
#endif
			lenStr(input, &count, LONG_MAX);
			count--;
		}
		while (!feof(stdin));
		printf("%ld\n", count);
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		count = arg_noSeparator ? 0 : 1;
		if (isOptionArg[i]) continue;
		if (arg_useFile) return lenFile(argv[i], &count);
		lenStr(argv[i], &count, LONG_MAX);
		printf("%ld\n", count);
	}
	return 0;
}

