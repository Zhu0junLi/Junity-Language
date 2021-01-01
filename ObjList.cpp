#include "ObjList.h"
#include "header.h"
#include "utils.h"
#include "vm.h"
#include <list>

using namespace std;

ObjList::ObjList(VM* vm, uint32_t size)
{
	init(vm, size);
}

void ObjList::init(VM* vm,uint32_t size)
{
	dataHead = dataTail = NULL;
	if (size > 0) {
		uint32_t index = 0;
		while (index < size) {
			push(vm, VALUE_NULL);
		}
	}
	header.init(vm, OT_LIST, vm->listClass);
}

void ObjList::insert(VM* vm, uint32_t index, Value value)
{
	if (index > count - 1) {
		RUNTIME_ERROR("Index out of list!");
		return;
	}
	ListNode* node=ALLOCATE(vm,ListNode);
	if (node == NULL) {
		MEM_ERROR("Malloc Failed");
		return;
	}
	ListNode* curNode = _find(index);
	node->next = curNode->pre;
	node->pre = curNode;
	curNode->next = curNode->next->pre = node;
	count++;
}

ListNode* ObjList::_find(uint32_t index)
{
	ListNode* curNode = dataHead;
	uint32_t i = 0;
	while (i < index) {
		curNode = curNode->next;
		i++;
	}
	return curNode;
}

Value ObjList::find(uint32_t index)
{
	return _find(index)->value;
}

void ObjList::push(VM* vm, Value value)
{
	ListNode* node = ALLOCATE(vm, ListNode);
	if (node == NULL) {
		MEM_ERROR("Malloc Failed");
		return;
	}
	dataTail->next = node;
	node->pre = dataTail;
	node->next = NULL;
	dataTail = node;
	count++;
}

void ObjList::remove(VM* vm, uint32_t index)
{
	ListNode* curNode = _find(index);
	curNode->next->pre = curNode->pre;
	curNode->pre->next = curNode->next;
	DEALLOCATE(vm, curNode);
}
