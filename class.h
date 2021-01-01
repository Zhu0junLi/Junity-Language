#pragma once
#include"utils.h"
#include"header.h"
#include"method.h"
#include"ObjString.h"

struct ObjString;

struct ObjHeader;

struct Method;

struct Class {
	ObjHeader *header;
	Class* super;
	uint32_t fieldNum;
	Buffer<Method> *methods;
	ObjString* name;
	void init();
};