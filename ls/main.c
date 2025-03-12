// main.c
// -------
// lists files in a dir
// ported from expl's ls: github.com/Matto58/explshell/blob/5312a16/main.py#L74
// also adds a couple of features expl's ls was missing (to be added soon?)
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "../globals.h"

// not bothering with some bsd blah blah blah stuff
#define DIRN 4
#define FILEN 8

#define OPT(opt) ((options & opt) == opt)
#define OPT_MONOTONE    0b00000001
#define OPT_SIMPLE      0b00000010
#define OPT_ONLYDIR     0b00000100
#define OPT_ONLYFL      0b00001000
unsigned char options = 0b00000000;


char *fs(long n) {
	char affixes[4] = {' ', 'K', 'M', 'G'};
	static char b[8];
	for (int i = 0; i < 4; i++) {
		if (n/1000 < 1) {
			sprintf(b, "%ld%c", n, affixes[i]);
			return b;
		}
		n /= 1000;
	}
	snprintf(b, 8L, "%ldT", n);
	return b;
}
/*
char *p2(int n) {
	char b[3] = "  ";
	if (n < 10) snprintf(b, 2, "0%d", n);
	else snprintf(b, 2, "%d", n);
	return b;
}
*/

int main(int argc, char **argv) {
	// index of the path parameter, or 0 if to use cwd
	int foundPathParam = 0;
	// process params and check for options
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (foundPathParam == 0) foundPathParam = i;
			else continue;
		}
		if (orCmpS(argv[i], "-m", "--monotone"))        options |= OPT_MONOTONE;
		else if (orCmpS(argv[i], "-s", "--simple"))     options |= OPT_SIMPLE;
		else if (orCmpS(argv[i], "-d", "--only-dirs"))  options |= OPT_ONLYDIR;
		else if (orCmpS(argv[i], "-f", "--only-files")) options |= OPT_ONLYFL;
		else if (orCmpS(argv[i], "-?", "--help")) {
			printf(
				"%s [options...] [path]\n"
				"lists files in a dir\n"
				"----\n"
				"options:\n"
				"  -m | --monotone   disable color\n"
				"  -s | --simple     only print file/dir names\n"
				"  -d | --only-dirs  shows only directories\n"
				"  -f | --only-files shows only files\n"
				"  -? | --help       shows this help\n"
				"----\n"
				"written by Matto58 and licensed under the MIT license\n"
				"report bugs/suggestions at https://github.com/BrilliantOS/mtutils/issues\n"
				"(english or czech)\n", argv[0]
			);
			return 0;
		}
		else printf("WARNING: unknown parameter %s, skipping\n", argv[i]);
	}

	char dirToScan[256];
	if (foundPathParam) strncpy(dirToScan, argv[foundPathParam], 256);
	else getcwd(dirToScan, 256);

	// header
	if (!OPT(OPT_SIMPLE)) {
		if (OPT(OPT_MONOTONE))
			printf("Type\tSize\tLast modified\tName\n");
		else
			printf("\e[30;107mType\tSize\tLast modified\tName\e[0m\n");
	}

	char *dc = OPT(OPT_MONOTONE) ? "" : "\e[0;96m";
	char *fc = OPT(OPT_MONOTONE) ? "" : "\e[0;91m";
	DIR *d;
	struct dirent *de;
	d = opendir(dirToScan);
	if (d == NULL) {
		perror("ls - opendir");
		return 1;
	}
	while ((de = readdir(d)) != NULL) {
		// printf("name: %s, type: %d\n", de->d_name, de->d_type);
		if (de->d_name[0] == '.') continue; // skip dotfiles but also . and ..
		int isDir = de->d_type == DIRN;

		if (isDir && OPT(OPT_ONLYFL)) continue;
		if (!isDir && OPT(OPT_ONLYDIR)) continue;

		if (OPT(OPT_SIMPLE)) {
			printf("%s%s%s ", isDir ? dc : fc, de->d_name, OPT(OPT_MONOTONE) ? "" : "\e[0m");
			continue;
		}

		struct stat s;
		char fullPath[256];
		snprintf(fullPath, 256, "%s/%s", dirToScan, de->d_name);
		if (stat(fullPath, &s) == -1) {
			perror("ls - stat");
			return 1;
		}
		// type
		printf("%s%s%s\t", isDir ? dc : fc, isDir ? "Dir" : "File", OPT(OPT_MONOTONE) ? "" : "\e[0m");

		// size
		printf("%s\t", isDir ? "" : fs(s.st_size));

		// last modified
		struct tm *lastMod = localtime(&(s.st_mtime));
		char lastModBuf[16];
		strftime(lastModBuf, 16, "D%y%m%d\tT%H%M%S", lastMod);
		printf("%s\t", lastModBuf);

		// name
		printf("%s%s%s\n", isDir ? dc : fc, de->d_name, OPT(OPT_MONOTONE) ? "" : "\e[0m");
	}
	closedir(d);
	if (OPT(OPT_SIMPLE)) printf("\n");
	return 0;
}
