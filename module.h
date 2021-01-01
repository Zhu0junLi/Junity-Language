#pragma once
#include "utils.h"
#include"Class.h"
#include"ObjString.h"
#include"vm.h"
#include"header.h"
#include<string>

struct Value;
struct VM;
struct ObjHeader;
template<class T>
struct Buffer;

struct ObjModule {
	ObjHeader header;
	Buffer<std::string> valueName;
	Buffer<Value> values;
	ObjString* moduleName;
	void init(VM* vm,const char* name);
};

struct ObjInstance {
	ObjHeader header;
	Value *values ;
	void init(VM* vm, Class* _class);
};