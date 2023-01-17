#include "include/io.h"

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

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
    FILE* fp= fopen(filename, "rb");
    char* line = NULL;
    size_t len = 0;
    size_t read;

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

void writeToFile(const char* filename, const char* data) {
    FILE* file = fopen(filename, "r+");
    if(file == 0) {
        printf("Unable to open the output file.");
        exit(-1);
    }
    file = fopen(filename, "w+");
    fwrite(data, sizeof(char), strlen(data), file);
    fflush(file);
    fclose(file);
    return;
}

void createFile(const char* filename) {

}