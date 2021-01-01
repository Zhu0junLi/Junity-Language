#include "ObjFunc.h"
#include"header.h"
struct ObjHeader;
void ObjFun::init(VM* vm, ObjModule* _moudle, uint32_t solt)
{
	header.init(vm,OT_FUNCTION,vm->funClass);
	constants.init();
	instrStream.init();
	moudle = _moudle;
	maxStackSoltUsedNum = solt;
	argsNum = 0;
	upvalueNum = 0;
}

void Upvalue::init(VM* vm, Value* _localValue)
{
	header.init(vm, OT_UPVALUE, NULL);
	localValue = _localValue;
	closedValue = VALUE_NULL;
	next = NULL;
}

void ObjClosure::init(VM* vm,ObjFun *_fun)
{
	header.init(vm, OT_CLOSURE, NULL);
	function = _fun;
	upvalues = ALLOCATE_ARRAY(vm, Upvalue*, _fun->upvalueNum);
	uint32_t i = 0;
	while (i < _fun->upvalueNum) {
		upvalues[i] = NULL;
		i++;
	}
}
