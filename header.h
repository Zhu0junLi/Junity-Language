#pragma once
#include"utils.h"

struct Class;
enum ObjType {
	OT_CLASS,
	OT_LIST,
	OT_MAP,
	OT_MODULE,//Ä£¿é
	OT_RANGE,
	OT_STRING,
	OT_UPVALUE,
	OT_FUNCTION,
	OT_CLOSURE,
	OT_INSTANCE,
	OT_THREAD,
	OT_UNKNOWN
};

struct ObjHeader
{
	ObjType type;
	bool dark;
	Class* _class;
	ObjHeader* next;
	ObjHeader() { type = OT_UNKNOWN, dark = false, _class = NULL, next = NULL; };
	ObjHeader(VM* vm,ObjType type,Class* __class);
	void init(VM* vm, ObjType _type, Class* __class);
};

enum ValueType {
	VT_OBJ,
	VT_NUM,
	VT_UNDEFINE,
	VT_NULL,
	VT_FALSE,
	VT_TRUE
};

struct Value {
	ValueType type;
	union {
		double num;
		ObjHeader objHeader;
	};
	Value() { type = VT_UNDEFINE, num = 0; };
	Value(ValueType _type, double _num = 0.0) { type = _type, num = _num; };
	Value(ValueType _type, ObjHeader header) { type = _type, objHeader = header; };
};

#define VT_TO_VALUE(vt) Value(vt)
#define BOOL_TO_VALUE(var) (VT_TO_VALUE(var?VT_TRUE:VT_FALSE))
#define NUM_TO_VALUE(num) Value(VT_NUM,num)
#define OBJ_TO_VALUE(obj) Value(VT_OBJ,obj.header)
#define VALUE_NULL VT_TO_VALUE(VT_NULL)
#define VALUE_TRUE VT_TO_VALUE(VT_TRUE)
#define VALUE_FALSE VT_TO_VALUE(VT_FALSE)