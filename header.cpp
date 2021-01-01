#include "header.h"
#include"vm.h"
struct VM;
ObjHeader::ObjHeader(VM* vm, ObjType _type, Class* __class)
{
	init(vm,_type,__class);
}


void ObjHeader::init(VM* vm, ObjType _type, Class *__class)
{
	type = _type;
	dark = false;
	next = vm->allObjHeader;
	vm->allObjHeader = this;
	_class = __class;
}
