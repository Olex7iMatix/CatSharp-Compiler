#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/io.h"
#include "include/cats.h"

char* OUTPUT_FILE = NULL;
char* SOURCE_FILE = NULL;

void parse_args(int argc, char** argv) {
    int source_file_count = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            if (i == argc) {
                printf("[CatSharp] Error: Missing output file");
                exit(1);
            }
            OUTPUT_FILE = argv[++i];
        } else if (!strcmp(argv[i], "--h")) {
            printf("=\t-=-\t-=-\tCatSharp\t-=-\t-=-\t=\n");
            printf("Compile: `%s <file> -o <out file>`\n", argv[0]);
            printf("=\t-=-\t-=-\tCatSharp\t-=-\t-=-\t=\n");
        
            exit(0);
        } else {
            if (i == argc) {
                printf("[CatSharp] Error: Missing source file");
                exit(1);
            }

            if (checkFileExists(argv[i]) == 1) {
                printf("[CatSharp] Error: Unrecognized argument: `%s`\n", argv[i]);
                exit(1);
            }

            if (SOURCE_FILE == NULL) {
                SOURCE_FILE = argv[i];
            } else {
                printf("[CatSharp] Error: Source file already exists!");
                exit(1);
            }
        }
    }
}

int main(int argc, char **argv) {
    parse_args(argc, argv);

    if (SOURCE_FILE == NULL) {
        printf("[CatSharp] Error: Please provide a source file. For help type `%s --h`\n", argv[0]);
        exit(1);
    }
    
    if (OUTPUT_FILE == NULL) {
        printf("[CatSharp] Error: Please provide a output file. For help type `%s --h`\n", argv[0]);
        exit(1);
    }

    cats_compile(SOURCE_FILE, OUTPUT_FILE);

    return 0;
}