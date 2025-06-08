#pragma once
#include "core/blx_defines.h"


typedef blxBool (*TestFunc)();

void testManager_Init();
void testManager_AddTest(TestFunc func);
void testManager_RunTests();