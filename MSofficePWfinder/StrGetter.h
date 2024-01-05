#pragma once
#include <Windows.h>

void setStrInit(const wchar_t* a);

void getStr(PWCHAR result, int threadIdx);

void nextStr(int threadIdx);