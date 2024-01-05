#include <Windows.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include "threadAmount.h"

#include <stdio.h>

#define STR_LEN 30
WCHAR lastStr[STR_LEN];
int quota[THREAD_AMOUNT];
WCHAR Strs[THREAD_AMOUNT][STR_LEN];

#define QUOTA_AMOUNT 5000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

WCHAR temp[STR_LEN];
void makeQuota(int threadIdx) {
	
	pthread_mutex_lock(&lock);

	wprintf(L"DONE : %s\n", lastStr);
	for (int i = 0; i < STR_LEN; i++)
		Strs[threadIdx][i] = temp[i] = 0;
	for (int i = 0; lastStr[i] != (WCHAR)'\0'; i++)
		Strs[threadIdx][i] = temp[i] = lastStr[i];

	int distance = QUOTA_AMOUNT;

	for (int i = 0; i < STR_LEN - 1; i++) {
		if (temp[i] == 0)
			distance--;
		else
			distance = temp[i] - 33 + distance;
		temp[i] = 33 + distance % 94;
		distance /= 94;
		if (distance == 0)
			break;
	}

	for (int i = 0; i < STR_LEN; i++)
		lastStr[i] = 0;
	for (int i = 0; temp[i] != (WCHAR)'\0'; i++)
		lastStr[i] = temp[i];

	quota[threadIdx] = QUOTA_AMOUNT - 1;

	pthread_mutex_unlock(&lock);
}

void nextStr(int threadIdx) {
	if (quota[threadIdx] == 0)
		makeQuota(threadIdx);
	else {
		// 33 - 126 => 전체 특수문자, 알파벳
		for (int i = 0; i < STR_LEN - 1; i++) {
			if (Strs[threadIdx][i] == 126)
				Strs[threadIdx][i] = 33;
			else {
				if (Strs[threadIdx][i] == 0)
					Strs[threadIdx][i] = 33;
				else
					Strs[threadIdx][i]++;
				break;
			}
		}

		quota[threadIdx]--;
	}
}

void setStrInit(const wchar_t* a) {
	for (int i = 0; i < STR_LEN; i++)
		lastStr[i] = 0;
	if (a != NULL) 
		for (int i = 0; a[i] != '\0'; i++) 
			lastStr[i] = a[i];

	for (int i = 0; i < THREAD_AMOUNT; i++)
		makeQuota(i);
}

void getStr(PWCHAR result, int threadIdx) {
	for (int i = 0; i < STR_LEN; i++)
		result[i] = 0;
	for (int i = 0; ; i++) {
		result[i] = Strs[threadIdx][i];
		if (Strs[threadIdx][i] == (WCHAR)'\0')
			break;
	}
}