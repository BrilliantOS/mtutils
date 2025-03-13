// main.c
// -------
// removes a directory or directories if multiple are specified
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include <unistd.h>

int main(int argc, char **argv) {
    int i = 1;
    while (i < argc) rmdir(argv[i++]);
}
