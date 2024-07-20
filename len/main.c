// main.c
// -------
// main program of the len mtutil
// written by Matto58 and licensed under the CC BY-SA 4.0 license
//
// github.com/BrilliantOS/mtutils

#include <stdio.h>
#include <string.h>
#include <limits.h>

// uncomment the line below if you'd like to see debug messages in stdout
#define DEBUG

// arg options, these are the defaults
int  arg_useFile     = 0;        // -f  | --use-file
char arg_separator   = ' ';      // -s  | --separator
long arg_limit       = LONG_MAX; // -l  | --limit
int  arg_noSeparator = 0;        // -ns | --no-separator
int  arg_showHelp    = 0;        // -?  | --help

size_t maxsize(size_t a, size_t b) {
	return a > b ? a : b;
}

// orCmp
// desc:
// 	compares a string with 2 other strings using OR
// args:
// 	const char *source - source string
// 	size_t srcLen      - length of the source string
// 	const char *matchA - first string to match against
// 	size_t lenA        - length of the first string
// 	const char *matchB - second string to match against
// 	size_t lenB        - length of the second string
// returns:
// 	1 if source and matchA match or if source and matchB match, otherwise 0
int orCmp(
	const char *source, size_t srcLen,
	const char *matchA, size_t lenA,
	const char *matchB, size_t lenB
) {
	return (
		strncmp(source, matchA, maxsize(srcLen, lenA)) == 0
		|| strncmp(source, matchB, maxsize(srcLen, lenB)) == 0
	);
}
// orCmpS
// desc:
//	simpler version of orCmp
// args:
//	const char *source - source string
//	const char *matchA - first string to match against
//	const char *matchB - second string to match against
// returns:
// 	1 if source and matchA match or if source and matchB match, otherwise 0
int orCmpS(const char *source, const char *matchA, const char *matchB) {
	return orCmp(
		source, strnlen(source, sizeof source),
		matchA, strnlen(matchA, sizeof matchA),
		matchB, strnlen(matchB, sizeof matchB)
	);
}

void lenStr(char *str, long *count) {
#ifdef DEBUG
	printf("DEBUG: entered lenStr with count=%d\n", *count);
#endif
	if (arg_noSeparator) {
#ifdef DEBUG
		printf("DEBUG: -ns specified\n");
#endif
		// a bit unsafe but it should be fine i think
		(*count) += strlen(str);
	}
	else {
		size_t len = strlen(str);
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

int lenFile(char *flname, long *count) {
#ifdef DEBUG
	printf("DEBUG: entered lenFile");
#endif
	FILE *fl = fopen((const char *)flname, "r");
	if (!fl) {
		printf("failed to open file \"%s\"", flname);
		return 1;
	}

	// work with a 256 byte buffer which should be good enough
	char buffer[256];
	while (!feof(fl)) {
		fgets(buffer, 256, fl);
		lenStr(buffer, count);
	}
	fclose(fl);
	printf("%d\n", *count);
	return 0;
}

int main(int argc, char **argv) {
	// first we process the args
	int argOffset;
	int isOptionArg[argc];
	for (argOffset = 1; argOffset < argc; argOffset++) {
		// skip non-options
		isOptionArg[argOffset] = 0;
		if (argv[argOffset][0] != '-') continue;
		else isOptionArg[argOffset] = 1;
		
		// pretend first argument is file name and not the string to process
		if (orCmpS(argv[argOffset], "-f", "--use-file"))
			arg_useFile = 1;
		// use different separator than space
		else if (orCmpS(argv[argOffset], "-s", "--separator"))
			arg_separator = argv[++argOffset][0];
		// set limit and exit with err code 1 if count exceeds limit
		else if (orCmpS(argv[argOffset], "-l", "--limit"))
			sscanf(argv[++argOffset], "%d", &arg_limit);
		// disallow separator and just count the character count
		else if (orCmpS(argv[argOffset], "-ns", "--no-separator"))
			arg_noSeparator = 1;
		// show help and exit
		else if (orCmpS(argv[argOffset], "-?", "--help"))
			arg_showHelp = 1;
		// cancel option scanning
		else if (!strncmp(argv[argOffset], "--",
			maxsize((size_t)2, sizeof argv[argOffset]))
		) {
			argOffset++;
			break;
		}
	}
	long count = 1;

	if (arg_showHelp) {
		// TODO
		printf("TODO: implement help\n");
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
	printf("DEBUG: arg parse finished, argOffset=%d, argc=%d, anyNormalArgs=%d\n", argOffset, argc, anyNormalArgs);
#endif

	if (arg_noSeparator) count--;
	if (!anyNormalArgs) {
		// FIXME: this code is kinda buggy and doesnt handle EOF when
		// some cli util (i.e. cat) pipes to this program correctly

		// 4kib of input should be sufficient
		char input[4096];
		int h = 0;
		int lim = 10000;
		do {
			fgets(input, 4096, stdin);
#ifdef DEBUG
			printf("DEBUG: inputted '%s'\n", input);
			printf("DEBUG: last char is %d\n", input[strlen(input)-2]);
#endif
			lenStr(input, &count);
			count--;
			h++;
		}
		while (input[strlen(input)-1] != EOF && h < lim);
		printf("%d\n", count);
		return 0;
	}

	if (arg_useFile) return lenFile(argv[argOffset], &count);
	for (int i = 1; i < argc; i++) {
		count = arg_noSeparator ? 0 : 1;
		if (isOptionArg[i]) continue;
		lenStr(argv[i], &count);
		printf("%d\n", count);
	}
	return 0;
}

