#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <io.h>
#include "KeyGetter.h"

/*=======================
���� �б� �Լ�
========================*/

FILE* file;
const int FILECONTENT_SIZE = 5000;
char fileContent[FILECONTENT_SIZE];
int readlen;
int index;
int endflag;

int setFile(char* filename) {
	index = -1;
	readlen = 0;
	endflag = 0;

	if (filename == NULL) {
		return -1;
	}
	else {
		char filePath[500];
		strcpy_s(filePath, USER_INPUT_DIR_SHORT);
		strcat_s(filePath, filename);
		if (0 != fopen_s(&file, filePath, "rb"))
			return -1;
		else
			return 0;
	}
}

void endFile() {
	fclose(file);
}

char getCharFromFile() {
	if (readlen == 0 || index >= readlen) {
		if (!feof(file)) {
			index = 0;
			readlen = fread(fileContent, sizeof(char), FILECONTENT_SIZE, file);
			return fileContent[index++];
		}
		else {
			endflag = 1;
			return -1;
		}
	}
	else {
		return fileContent[index++];
	}
}

int isEof() {
	return endflag;
}

/*==============================
���� �������� �ʿ����� ���� �Լ�
===============================*/

WCHAR** getDataInFile(char* filename) {
	if (0 != setFile(filename))
		return NULL;

	int currentKeyword = 0;
	int keywordIdx = 0;
	const int KEYWORDS_LEN = 5;
	const char* keywords[] = {
		"p:encryptedKey",
		"spinCount",
		"saltValue",
		"encryptedVerifierHashInput",
		"encryptedVerifierHashValue"
	};

	WCHAR** findedValues = (WCHAR**)malloc((KEYWORDS_LEN-1) * sizeof(WCHAR*));
	for (int i = 0; i < (KEYWORDS_LEN - 1); i++)
		findedValues[i] = (WCHAR*)malloc(500 * sizeof(WCHAR));

	while (true) {
		if (isEof()
			|| currentKeyword == KEYWORDS_LEN)
			break;
		else if (keywords[currentKeyword][keywordIdx] == '\0') {
			if (currentKeyword != 0) {
				while (getCharFromFile() != '"');
				int i = 0;
				char a;
				while (true) {
					a = getCharFromFile();
					if (a != '"')
						findedValues[currentKeyword - 1][i++] = (WCHAR)a;
					else {
						findedValues[currentKeyword - 1][i] = (WCHAR)'\0';
						break;
					}
				}
			}
			currentKeyword++;
			keywordIdx = 0;
		}
		else if (getCharFromFile() == keywords[currentKeyword][keywordIdx]) {
			keywordIdx++;
		}
		else {
			keywordIdx = 0;
		}
	}
	if (currentKeyword == KEYWORDS_LEN) {
		return findedValues;
	}
	else {
		return NULL;
	}

	endFile();
}

/*===============================
���Ϻ� Ű ���� �� ����� ���� Ȯ��
================================*/

void recordFileData(fileData** root,char* filename, WCHAR** keyData) {
	fileData* newfn = (fileData*)malloc(sizeof(fileData));
	strcpy_s(newfn->filename, filename);
	newfn->nextFile = NULL;
	newfn->keyData = keyData;

	if (*root == NULL)
		*root = newfn;
	else {
		fileData* current = *root;
		while (true) {
			if (current->nextFile == NULL)
				break;
			else
				current = current->nextFile;
		}
		current->nextFile = newfn;
	}
}

WCHAR** getDataFromFile() {
	struct _finddata_t finddata;
	long handle;
	int result = 1;

	char path[500];
	strcpy_s(path, USER_INPUT_DIR_SHORT);
	strcat_s(path, "*.*");

	fileData* fileNames = NULL;

	// ��ȿ�� ���� ��� �ܾ����
	handle = _findfirst(path, &finddata);
	while (true)
	{
		if (strcmp(USER_INPUT_FILE, finddata.name) != 0
			&& (finddata.attrib & _A_SUBDIR) == 0) {
			WCHAR** passwordData = getDataInFile(finddata.name);
			if (passwordData != NULL)
				recordFileData(&fileNames, finddata.name, passwordData);
		}
		
		result = _findnext(handle, &finddata);
		if (result == -1)
			break;
	}
	_findclose(handle);

	// ������ ���� ���
	if (fileNames == NULL) {
		printf("\n\n");
		printf("  �� ��ȣ�� ������ ������ ������ �־��ּ���!\n  (��ȣ�� �ɸ� ������ �ϳ��� ã�� �� ����. Ȥ�� ���ϵ��� �ջ��.)\n  (��ġ : ���α׷��� �ִ� ������ input ���� ���ο�)\n");
		return NULL;
	}

	// ������ ������ ���
	else if (fileNames->nextFile == NULL) {
		printf("\n\n");
		printf("  �� ��ȣ ������ �ϳ��� �߰ߵǾ� �ٷ� �����մϴ�. : %s\n",fileNames->filename);
		return fileNames->keyData;
	}

	// ������ 2�� �̻�
	else {
		char userInput[50];
		fileData* root = fileNames;
		while (true) {
			printf("\n\n");
			printf("  ��ȣ ������ ������ �߰ߵǾ����ϴ�.\n");
			printf("  - ���� : %s �� �³���? (y/n)\n  ", root->filename);
			scanf_s("%s", userInput,50);

			if (strcmp(userInput, "y") == 0) {
				printf("\n");
				printf("  �� ���� : %s ���� �����մϴ�.\n", root->filename);
				return root->keyData;
			}
			else if (strcmp(userInput, "n") == 0) {
				root = root->nextFile;
				if (root == NULL) {
					printf("\n");
					printf("  �� �� �̻� ã�� �� �ִ� ������ �����ϴ�!\n");
					return NULL;
				}

				system("cls");
			}
			else {
				system("cls");

				printf("\n");
				printf("  �� [%s]-�߸��� �Է� : �ٽ� �Է����ּ���.\n",userInput);
			}
		}
	}
}

/*===============
���� Ű ���� ��ȯ
=================*/

KeyData* getKeyData() {
	WCHAR** keys = getDataFromFile();
	if (keys == NULL)
		return NULL;

	KeyData * kd = (KeyData*)malloc(sizeof(KeyData));
	int r = 0;
	for (int i = 0; keys[0][i] != '\0'; i++)
		r = r * 10 + (keys[0][i] - (WCHAR)'0');
	kd->spinCount = r;
	kd->Salt = keys[1];
	kd->EncryptedVerifierHashInput = keys[2];
	kd->EncryptedVerifierHashValue = keys[3];

	return kd;
}