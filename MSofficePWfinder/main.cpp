#include <stdio.h>
#include <tchar.h>
#include "threadCryptor.h"


#include "KeyGetter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// test용
	readFile();
	getchar();
	return 0;


	PWCHAR password = getPassword((PWCHAR)L"804\"!!!");
	wprintf(L"password : [%s]\n          FINDED\n",password);
	return 0;
}