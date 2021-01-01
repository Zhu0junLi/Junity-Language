#include "ObjString.h"
#include"vm.h"
#include"header.h"

#include<string>
using namespace std;

uint32_t ObjString::HashCode()
{
    uint32_t code = 2166136261, i = 0,len=(uint32_t)value.length();
    while (i < len) {
        code ^= value[i];
        code *= 16777619;
        i++;
    }
    hashCode = code;
    return code;
}



ObjString::ObjString(VM* vm, const char* str, uint32_t length)
{
    init(vm, str, length);
}

void ObjString::init(VM* vm, const char* str, uint32_t length)
{
    ASSERT(length == 0 || str != NULL, "String's length doesn't match!");
    vm->allocateBytes += length + 8 + 1;//8为string所占空间
    header.init(vm, OT_STRING, vm->stringClass);
    hashCode = 0;
    if (str != NULL) {
        char* data = (char*)malloc((size_t)length + 1);
        if (data == NULL) {
            MEM_ERROR("Malloc Failed!");
            return;
        }
        memcpy(data, str, length);
        data[length] = '\0';
        value = string(data);
        HashCode();
        free(data);
    }
}
