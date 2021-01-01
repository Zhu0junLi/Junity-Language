#pragma once
#include"header.h"
#include"vm.h"

struct VM;
struct Value;

enum MethodType {
	MT_NULL,
	MT_PRIMITIVE,
	MT_SCRIPT,
	MT_FN_CALL
};

typedef bool (*Primitive)(VM* vm, Value* args);

struct Method {
	MethodType type;
};