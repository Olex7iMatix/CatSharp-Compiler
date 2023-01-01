#ifndef IO
#define IO

int checkFileExists(const char* filename);

char* getFileContent(const char* filename);

void writeToFile(const char* filename, const char* content);

void createFile(const char* filename);

#endif