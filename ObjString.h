#pragma once
#include"header.h"
#include<string>

struct ObjString {
	ObjHeader header;
	std::string value;
	uint32_t hashCode;
	uint32_t HashCode();
	ObjString() { value = "", hashCode = 0; };
	ObjString(VM* vm,const char* str,uint32_t length);
	void init(VM* vm, const char* str, uint32_t length);
};