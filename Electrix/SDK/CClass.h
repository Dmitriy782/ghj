#pragma once

#include "ClassId.h"
#include "Recv.h"

struct CClass final 
{
    void* createFunction;
    void* createEventFunction;
    char* networkName;
    RecvTable* recvTable;
    CClass* next;
    ClassId classId;
};