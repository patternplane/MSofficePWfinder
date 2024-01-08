#include <stdio.h>
#include <tchar.h>
#include "threadCryptor.h"


#include "KeyGetter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	PWCHAR password = getPassword((PWCHAR)L"test");//PWCHAR password = getPassword((PWCHAR)L"804\"!!!");
	if (password != NULL) {
		wprintf(L"password : [%s]", password);
		printf("\n           성공!\n");
	}
	else
		printf("실패.\n");

	return 0;
}