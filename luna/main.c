// main.c
// -------
// main program of the luna util
// written by Matto58 and licensed under the CC BY-SA 4.0 license
//
// github.com/BrilliantOS/mtutils

#include "../globals.h"
#include <dirent.h>
#include <unistd.h>

// 512 bytes is fine; windows uses 256 and linux 4096 but that's a bit overkill
#define PATH_MAX_LENGTH 512

char activeDir[PATH_MAX_LENGTH];

int main(int argc, char **argv) {
	getcwd(activeDir, PATH_MAX_LENGTH);

	DIR *sourceDir;
	struct dirent *dirEntry;
	sourceDir = opendir(activeDir);
	if (!sourceDir) {
		printf("failed to open dir %s\n", activeDir);
		return 1;
	}
	// running through dir entries twice, once to count how many
	// files we have and then to store our file/dir names (is this fixable
	// or is it just a minor inconvenience?)
	size_t dirEntsSize = 0;
	while ((dirEntry = readdir(sourceDir)) != NULL)
		dirEntsSize++;
	
	// rewinding and storing the entries in an array...
	rewinddir(sourceDir);
	char *dirEnts[dirEntsSize];
	int i = 0;
	while ((dirEntry = readdir(sourceDir)) != NULL)
		dirEnts[i++] = dirEntry->d_name;
	// close after we've done that, we want to let everyone else use the
	// directory too
	closedir(sourceDir);
	// we don't need the dir entry now, we have the names stored
	free(dirEntry);
	
	// and now to bubble sort our entries
	// TODO: implement quick/selection sort instead of bubble sort
	// commented out because the function is broken. excellent
	// bubbleSortStrsKS(dirEnts, dirEntsSize, 0);
	
	// TODO: test code, delete this later
	for (int i = 0; i < dirEntsSize; i++) {
		printf("%s\n", dirEnts[i]);
	}
	return 0;
}
