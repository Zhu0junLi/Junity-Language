#include "vm.h"

void VM::init()
{
	allocateBytes = 0;
	allObjHeader = NULL;
	curParser = NULL;
}

VM::VM()
{
	this->init();
}
