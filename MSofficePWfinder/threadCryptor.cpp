#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <Windows.h>
#include "cryptor.h"
#include "StrGetter.h"
#include "threadAmount.h"
#include "KeyGetter.h"

#include <stdio.h>

int isFinded = 0;
PWCHAR findedPassword;

void* checkPasswordByThread(void* threadIdx) {
	PWCHAR password = checkCorrectPassword((*((int*)threadIdx)));

	if (password != NULL) {
		findedPassword = password;
		isFinded = 1;
	}
	
	pthread_exit(NULL);
	return NULL;
}

PWCHAR getPassword(PWCHAR startStr) {
	KeyData* kd = getKeyData();
	if (kd == NULL)
		return NULL;
	else
		setKeyData(kd);

	setStrInit(startStr);
	
	pthread_t threads[THREAD_AMOUNT];
	for (int i = 0; i < THREAD_AMOUNT; i++) {
		int* param = (int*)malloc(sizeof(int));
		*param = i;
		pthread_create(&(threads[i]), NULL, checkPasswordByThread, param);
		pthread_detach(threads[i]);
	}
	
	int cnt = 0;
	while (isFinded == 0)
		if ((cnt++)%10000 == 0)
			printf("");

	return findedPassword;
}