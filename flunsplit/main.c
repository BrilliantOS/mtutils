// main.c
// -------
// reverses what flsplit does
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("no file name specified\n");
        return 1;
    }

    // TODO: REVISE THIS WHOLE THING. i just botched together something to make things work.
    //       this code is fucking ugly. my eyesss

    // get the first chunk for the buffer size
    // todo: use a fixed buffer size and don't rely on the chunks to be the same size
    char chunkFilename[256];
    snprintf(chunkFilename, 256, "%s.0", argv[1]);
    FILE *chunk = fopen(chunkFilename, "rb");
    if (chunk == NULL) return 1;
    FILE *outputFile = fopen(argv[1], "wb");
    // figure out first chunk size
    fseek(chunk, 0, SEEK_END);
    long bufferSize = ftell(chunk);
    void *buffer = malloc(bufferSize);
    long chunkCount = 1;
    fseek(chunk, 0, SEEK_SET);
    fread(buffer, 1, bufferSize, chunk);
    fwrite(buffer, 1, bufferSize, outputFile);
    fclose(chunk);
    snprintf(chunkFilename, 256, "%s.1", argv[1]);
    while (chunk = fopen(chunkFilename, "rb")) {
        size_t read;
        while ((read = fread(buffer, 1, bufferSize, chunk)) > 0)
            fwrite(buffer, 1, read, outputFile);
        chunkCount++;
        snprintf(chunkFilename, 256, "%s.%ld", argv[1], chunkCount);
        fclose(chunk);
    }

    fclose(outputFile);
    free(buffer);
    return 0;
}
