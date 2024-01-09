#pragma once
#include <windows.h>

// delete, add, change
// single or double
enum currentStep {
	original,
	d1,
	a1,
	c1,
	d2,
	a2,
	c2,
	d1a1,
	d1c1,
	a1c1,
	end
};

#define QUOTA_AMOUN 2*500 // 2의 배수일 것
#define CHAR_LEN 100

struct quotaData {
	WCHAR quota[QUOTA_AMOUN][CHAR_LEN];
	int amount;
};

void strInit(const WCHAR* initStr);

void getQuotaData(quotaData* destination);