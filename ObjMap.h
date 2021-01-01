#pragma once
#include"header.h"
#include<map>
using namespace std;
struct ObjMap {
	ObjHeader *header;
	map<ObjHeader,ObjHeader> data;
	void init(VM* vm, uint32_t size);
};