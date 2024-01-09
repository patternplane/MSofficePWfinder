#include <stdio.h>
#include <tchar.h>
#include "threadCryptor.h"

#include "KeyGetter.h"
#include "DicGetter.h"
#include "StrGetter2.h"

#include "paths.h"
#include "time.h"
#include <direct.h>
#include <io.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// 입출력 디렉토리 검사
	if (0 != _access(OUTPUT_DIR, 0)) {
		_mkdir(OUTPUT_DIR);
	}
	if (0 != _access(USER_INPUT_DIR_SHORT, 0)) {
		_mkdir(USER_INPUT_DIR_SHORT);
	}

	// 출력
	FILE* fp;
	char outFileName[100];
	char currentTime[100];
	time_t ct;
	ct = time(NULL);
	sprintf_s(currentTime, "%d", ct);
	strcpy_s(outFileName, OUTPUT_DIR);

	// 패스워드 검색
	PWCHAR password = getPassword();
	if (password != NULL) {
		password;
		wprintf(L"password : [%s]", password);
		printf("\n           성공!\n");

		strcat_s(outFileName, OUTPUT_FILE_SUCCESS);
		strcat_s(outFileName, currentTime);
		strcat_s(outFileName, OUTPUT_FILE_END);
		fopen_s(&fp, outFileName, "w");
		fwprintf(fp,L"%s",password);
		fclose(fp);
	}
	else {
		printf("실패.\n");

		strcat_s(outFileName, OUTPUT_FILE_FAIL);
		strcat_s(outFileName, currentTime);
		strcat_s(outFileName, OUTPUT_FILE_END);
		fopen_s(&fp, outFileName, "w");
		printf("실패");
		fclose(fp);
	}

	while (getchar() != '\n');
	getchar();
	return 0;
}