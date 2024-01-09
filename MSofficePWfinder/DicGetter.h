#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "KeyGetter.h"

struct DictionData {
	int amount;
	PWCHAR* diction;
};

DictionData* getDiction();