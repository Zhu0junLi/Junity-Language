#include "core.h"
#include "utils.h"
#include<stdio.h>
#include<sys/stat.h>
char* rootDir = NULL;
char* ReadFile(const char* path)
{
    size_t length = 0;
    char* source = NULL;
    struct stat filestat;
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        IO_ERROR("Cannot open file:\"%s\"", path);
        return NULL;
    }
    stat(path, &filestat);
    length = filestat.st_size;
    source = (char*)malloc(length + 1);
    if (source == NULL) {
        MEM_ERROR("Malloc failed!");
        return NULL;
    }
    size_t readNum = fread(source, sizeof(char), length, fp);
    if (readNum < length)
    {
        IO_ERROR("Cannot read file:\"%s\",%d,%d.\n%s", path,readNum,length,source);
    }
    source[readNum] = '\0';
    fclose(fp);
    return source;

}
