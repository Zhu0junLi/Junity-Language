#include "module.h"
#include "vm.h"
struct VM;

#include<string.h>

void ObjModule::init(VM* vm, const char* name)
{
	header.init(vm, OT_MODULE, NULL);
	valueName.init();
	values.init();
	moduleName = NULL;
	if (name != NULL) {
		moduleName = &ObjString(vm,name,(uint32_t)strlen(name));
	}
}

void ObjInstance::init(VM* vm, Class* _class)
{
	values = ALLOCATE_ARRAY(vm, Value, _class->fieldNum);
	header.init(vm, OT_CLASS, _class);
	uint32_t i = 0;
	while (i < _class->fieldNum) {
		values[i] = VALUE_NULL;
	}
}
