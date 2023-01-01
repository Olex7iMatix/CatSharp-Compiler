#include "include/io.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int checkFileExists(const char* filename) {
    if( access( filename, F_OK ) != -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

char* getFileContent(const char* filename) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Could not read file `%s`\n", filename);
        exit(1);
    }
    char* buffer = (char*) calloc(1, sizeof(char));
    buffer[0] = '\0';

    while ((read = getline(&line, &len, fp)) != -1) {
        buffer = (char*) realloc(buffer, (strlen(buffer) + strlen(line) + 1) * sizeof(char));
        strcat(buffer, line);
    }

    fclose(fp);
    if (line) free(line);
    return buffer;
}

void writeToFile(const char* filename, const char* content) {
    if (checkFileExists(filename) == 1) {
        FILE* f;
        f = fopen(filename, "w");
        if(f == NULL)
        {
            printf("[CatSharp] Error: Unable to create file `%s`.\n", filename);
            exit(1);
        }
        free(f);
    }

    FILE* fp;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Could not open file `%s` (Write Purpose)\n", filename);
        exit(1);
    }

    fputs(content, fp);

    fclose(fp);
}

void createFile(const char* filename) {

}