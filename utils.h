#pragma once
#include"common.h"

struct VM;
struct Parser;
void* memManager(VM* vm, void* ptr, uint32_t oldSize, uint32_t newSize);

uint32_t ceilToPowerOf2(uint32_t v);

enum ErrorType
{
	ERROR_IO,
	ERROR_MEM,
	ERROR_LEX,
	ERROR_COMPILE,
	ERROR_RUNTIME
};

void ErrorReport(Parser* paresr, ErrorType errorType, const char* fmt, ...);

#define ALLOCATE(vmPtr,type) \
	(type*)memManager(vmPtr,NULL,0,sizeof(type))

#define ALLOCATE_EXTRA(vmPtr,mainType,extraSize) \
	(type*)memManager(vmPtr,NULL,0,sizeof(mainType)+extraSize)

#define ALLOCATE_ARRAY(vmPtr,type,count) \
	(type*)memManager(vmPtr,NULL,0,count*sizeof(type))

#define DEALLOCATE_ARRAY(vmPtr,arrayPtr,count) \
	memManager(vmPtr,arrayPtr,sizeof(arrayPtr[0])*count,0)

#define DEALLOCATE(vmPtr,memPtr) \
	memManager(vmPtr,memPtr,0,0);
template<class T>
struct Buffer {
	T* datas;
	uint32_t count;
	uint32_t capacity;
	Buffer() {
		init();
	}
	void init()
	{
		datas = NULL;
		count = capacity = 0;
	}
	void FillWrite(VM* vm, T data, uint32_t fillcount)
	{
		uint32_t newCounts = count + fillcount;
		if (newCounts > capacity) {
			uint32_t  oldSize = capacity * sizeof(T);
			capacity = ceilToPowerOf2(newCounts);
			uint32_t newSize = capacity * sizeof(T);
			ASSERT(newSize > oldSize, "Faint...memory allocate!");
			datas = (T*)memManager(vm, datas, oldSize, newSize);
		}
		for (uint32_t i = 0; i < fillcount; i++)
			datas[count++] = data;
	}
	int Add(VM* vm, T data)
	{
		FillWrite(vm, data, 1);
		return count;
	}
	void Clear(VM* vm)
	{
		uint32_t oldSize = capacity * sizeof(T);
		memManager(vm, datas, oldSize, 0);
		init();
	}
};

typedef uint32_t Byte;
typedef char Char;
typedef int Int;



#define ISSPACE(c) (c=='\n'||c=='\r'||c=='\t'||c==' ')
#define ISLOWWER(c) (c>='a'&&c<='z')
#define ISUPPER(c) (c>='A'&&c<='Z')
#define ISALPHA(c) ((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
#define ISNUM(c) (c>='0'&&c<='9')
#define ISXNUM(c) ((c>='0'&&c<='9')||(c>='A'&&c<='F')||(c>='a'&&c<='f'))
#define ISPLNUM(c) ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))
#define ISWORD(c) ((~(c >> 8) == 0))

#define IO_ERROR(...) ErrorReport(NULL,ERROR_IO,__VA_ARGS__)
#define MEM_ERROR(...) ErrorReport(NULL,ERROR_MEM,__VA_ARGS__)
#define LEX_ERROR(parser,...) ErrorReport(parser,ERROR_LEX,__VA_ARGS__)
#define COMPILE_ERROR(parser,...) ErrorReport(parser,ERROR_COMPILE,__VA_ARGS__)
#define RUNTIME_ERROR(...) ErrorReport(NULL,ERROR_RUNTIME,__VA_ARGS__)

#define DEFAULT_BUFFER_SIZE 512