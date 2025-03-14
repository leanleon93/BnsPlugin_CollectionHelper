#pragma once
#include "BSFunctions.h"
#include "Data.h"

extern DrEl* (__fastcall* oFind_b8AutoId)(DrMultiKeyTable* thisptr, unsigned __int64 key);
DrEl* __fastcall hkFind_b8AutoId(DrMultiKeyTable* thisptr, unsigned __int64 key);

extern World* (__fastcall* BNSClient_GetWorld)();