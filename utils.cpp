#include "utils.h"
#include"vm.h"
#include"parser.h"
#include<stdio.h>

void* memManager(VM* vm, void* ptr, uint32_t oldSize, uint32_t newSize)
{
    vm->allocateBytes += newSize - oldSize;
    if (newSize == 0) {
        free(ptr);
        return NULL;
    }
    return realloc(ptr, newSize);
}

uint32_t ceilToPowerOf2(uint32_t v)
{
    v += (v == 0);
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

void ErrorReport(Parser* paresr, ErrorType errorType, const char* fmt, ...)
{
    char buffer[DEFAULT_BUFFER_SIZE] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, DEFAULT_BUFFER_SIZE, fmt, ap);
    va_end(ap);
    switch (errorType) {
    case ERROR_IO:
    case ERROR_MEM:
        fprintf(stderr, "%s:%d In function %s():%s",
            __FILE__, __LINE__, __func__, buffer);
        break;
    case ERROR_LEX:
    case ERROR_COMPILE:
        ASSERT(parse != NULL, "Parse is null!");
        fprintf(stderr, "Error! \"%s\" line %d:%s\n", paresr->file, paresr->line, buffer);
        break;
    case ERROR_RUNTIME:
        fprintf(stderr, "%s\n", buffer);
        break;
    default:
        NOTREACHED();
    }
    exit(0);
}

