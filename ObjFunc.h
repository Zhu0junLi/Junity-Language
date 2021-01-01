#pragma once
#include"utils.h"
#include "header.h"
#include"module.h"
struct Value;
struct VM;
struct ObjHeader;
template<class T>
struct Buffer;
struct ObjMoudle;

struct ObjFun {
	ObjHeader header;
	Buffer<Value> constants;
	Buffer<Byte> instrStream;
	ObjModule* moudle;
	uint32_t maxStackSoltUsedNum;
	uint32_t argsNum;
	uint32_t upvalueNum;
	void init(VM* vm,ObjModule* _moudle,uint32_t solt);
};

struct Upvalue {
	ObjHeader header;
	Value* localValue;
	Value closedValue;
	Upvalue* next;
	void init(VM* vm, Value* _localValue);
};

struct ObjClosure {
	ObjHeader header;
	ObjFun* function;
	Upvalue** upvalues;
	void init(VM* vm,ObjFun *_fun);
};

struct Frame {
	uint8_t* ip;
	ObjClosure closure;
	Value* stack;
};