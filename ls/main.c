// main.c
// -------
// lists files in a dir
// ported from expl's ls: github.com/Matto58/explshell/blob/5312a16/main.py#L74
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

// not bothering with some bsd blah blah blah stuff
#define DIRN 4
#define FILEN 8

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
	sprintf(b, "%ldT", n);
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
	char dirToScan[256];
	if (argc > 1) strncpy(dirToScan, argv[1], 256);
	else getcwd(dirToScan, 256);

	// header
	printf("\e[30;107mType\tSize\tLast modified\tName\e[0m\n");
	
	char *dc = "\e[0;96m";
	char *fc = "\e[0;91m";
	DIR *d;
	struct dirent *de;
	d = opendir(dirToScan);
	if (d == NULL) {
		perror("ls - opendir");
		return 1;
	}
	while ((de = readdir(d)) != NULL) {
		// printf("name: %s, type: %d\n", de->d_name, de->d_type);
		if (de->d_name[0] == '.') continue;
		int isDir = de->d_type == DIRN;
		struct stat s;
		char fullPath[256];
		snprintf(fullPath, 256, "%s/%s", dirToScan, de->d_name);
		if (stat(fullPath, &s) == -1) {
			perror("ls - stat");
			return 1;
		}
		
		// type
		printf("%s%s\e[0m\t", isDir ? dc : fc, isDir ? "Dir" : "File");

		// size
		printf("%s\t", isDir ? "" : fs(s.st_size));

		// last modified
		struct tm *lastMod = localtime(&(s.st_mtime));
		char lastModBuf[16];
		strftime(lastModBuf, 16, "D%y%m%d\tT%H%M%S", lastMod);
		printf("%s\t", lastModBuf);

		// name
		printf("%s%s\e[0m\n", isDir ? dc : fc, de->d_name);
	}
	closedir(d);
	return 0;
}
