#pragma once
#include"common.h"

struct Class;
struct Parser;
struct ObjHeader;

struct VM {
	Class* stringClass;
	Class* funClass;
	Class* listClass;
	uint32_t allocateBytes;
	Parser* curParser;
	ObjHeader *allObjHeader;
	void init();
	VM();
};