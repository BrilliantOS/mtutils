// main.c
// -------
// splits a file into multiple parts
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include <stdio.h>
#include <stdlib.h>

// buffer size for copying the file content
#define BUF_SIZE 256
// minimum chunk size
// yes, this is probably useless but still
#define MIN_CHUNK_SIZE 16

// stolen from ../globals.h
// not including that file however so i don't bloat the binary
size_t minsize(size_t a, size_t b) {
	return a < b ? a : b;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("too little args, expected <chunk size> <file path>\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[2], "rb");
    void *buffer = malloc(BUF_SIZE);
    long chunkSize = -1;
    printf("askjdajsghdaksjd 1\n");
    sscanf(argv[1], "%ld", &chunkSize);
    printf("askjdajsghdaksjd 2\n");
    printf("chunkSize = %ld\n", chunkSize);
    printf("askjdajsghdaksjd 3\n");

    if (inputFile == NULL) {
        perror("flsplit - fopen");
        return 1;
    }
    if (chunkSize < MIN_CHUNK_SIZE || chunkSize < 0) {
        printf("error: too small chunks (must be at least %ld bytes big)", MIN_CHUNK_SIZE);
        return 1;
    }

    // figure out file size by going to the end of the file and getting the position
    fseek(inputFile, 0, SEEK_END);
    long inputFileSize = ftell(inputFile);
    // of course, don't forget to rewind
    fseek(inputFile, 0, SEEK_SET);

    long chunkCount = 0;
    while (ftell(inputFile) < inputFileSize) {
        char chunkFilename[256];
        snprintf(chunkFilename, 256, "%s.%ld", argv[2], chunkCount);
        FILE *chunk = fopen(chunkFilename, "wb");
        long chunkBytes = 0;
        long writtenBytes = 0;
        while (writtenBytes < chunkSize) {
            // read only the amount of bytes necessary - hence we call minsize
            // (this function checks 2 size_ts and returns the smaller one)
            // so we know that we're reading just enough what the buffer can handle
            // and also enough for the chunk
            size_t readBytes = fread(buffer, 1, minsize(chunkSize - chunkBytes, BUF_SIZE), inputFile);
            fwrite(buffer, 1, readBytes, chunk);
            writtenBytes += readBytes;
            //printf("meow chunkCount=%ld writtenBytes=%ld chunkSize=%ld readBytes=%ld\n", chunkCount, writtenBytes, chunkSize, readBytes);
            if (readBytes == 0) break; // don't forget ffs
        }
        fclose(chunk);
        chunkCount++;
    }
    free(buffer);
    fclose(inputFile);
    
    return 0;
}
