#include "DicGetter.h"

DictionData* getDiction() {
	FILE* fp;
	char filePath[500];
	strcpy_s(filePath, USER_INPUT_DIR_SHORT);
	strcat_s(filePath, USER_INPUT_FILE);
	int errorCode = fopen_s(&fp, filePath,"r");

	if (errorCode != 0) {
		printf("\n\n");
		printf("  �� ���� ��й�ȣ�� ������ ������ �غ����ּ���!\n  (��ġ : ���α׷��� �ִ� ������ input ���� ���ο� input.txt ������ �����ϱ�)\n");
		return NULL;
	}

	char fileContent[2500];
	int readLen = fread(fileContent,sizeof(char),5000,fp);
	fileContent[readLen] = '\0';

	char* passwords[100];
	int cnt = 0;
	char* temp;
	char* token = strtok_s(fileContent, "\n", &temp);
	while (token != NULL) {
		passwords[cnt++] = token;
		token = strtok_s(NULL, "\n", &temp);
	}

	DictionData* result = (DictionData*)malloc(sizeof(DictionData));
	result->amount = cnt;
	result->diction = (PWCHAR*)malloc(cnt * sizeof(PWCHAR));
	for (int i = 0; i < cnt; i++) {
		int len = 0;
		for (; passwords[i][len] != '\0'; len++);
		result->diction[i] = (PWCHAR)malloc((len + 1) * sizeof(WCHAR));
		for (int j = 0; ; j++) {
			result->diction[i][j] = (WCHAR)(passwords[i][j]);
			if (passwords[i][j] == '\0')
				break;
		}
	}

	return result;
}