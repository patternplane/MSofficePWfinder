#include <stdio.h>
#include <tchar.h>
#include "threadCryptor.h"

int _tmain(int argc, _TCHAR* argv[])
{
	PWCHAR password = getPassword((PWCHAR)L"804\"!!!");
	wprintf(L"password : [%s]\n          FINDED\n",password);
	return 0;
}