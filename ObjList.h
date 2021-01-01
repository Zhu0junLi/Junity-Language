#pragma once
#include "header.h"
#include "vm.h"
#include <list>

struct ListNode {
	Value value;
	ListNode* next;
	ListNode* pre;

};

struct ObjList {
	ObjHeader header;
	ListNode* dataHead;
	ListNode* dataTail;
	uint32_t count;
	ObjList(VM* vm, uint32_t size);
	void init(VM* vm,uint32_t size);
	void insert(VM* vm, uint32_t index, Value value);
	void push(VM* vm, Value value);
	void remove(VM* vm, uint32_t index);
	Value find(uint32_t index);
private:
	ListNode* _find(uint32_t index);
};