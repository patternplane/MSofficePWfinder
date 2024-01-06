#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <io.h>
#include "KeyGetter.h"

KeyData* get() {
	KeyData* kd = (KeyData*)malloc(sizeof(KeyData));
	kd->isValid = 0;

	FILE* f;
	//fopen_s(&f,);

	return kd;
}

void saveFileName(fileName** root,char* filename) {
	fileName* newfn = (fileName*)malloc(sizeof(fileName));
	strcpy_s(newfn->filename, filename);
	newfn->nextFile = NULL;

	if (*root == NULL)
		*root = newfn;
	else {
		fileName* current = *root;
		while (true) {
			if (current->nextFile == NULL)
				break;
			else
				current = current->nextFile;
		}
		current->nextFile = newfn;
	}
}

char* getUserInputFile() {
	struct _finddata_t finddata;
	long handle;
	int result = 1;

	char path[500];
	strcpy_s(path, USER_INPUT_DIR_SHORT);
	strcat_s(path, "*.*");

	fileName* fileNames = NULL;

	handle = _findfirst(path, &finddata);
	if (handle == -1) {
		printf("��ȣ�� ������ ������ ������ �־��ּ���!\n(��ġ : (���α׷��� �ִ� ������ input ���� ���ο�)\n");
		return NULL;
	}
	while (true)
	{
		if (strcmp(USER_INPUT_FILE, finddata.name) != 0
			&& (finddata.attrib & _A_SUBDIR) == 0)
			saveFileName(&fileNames, finddata.name);
		
		result = _findnext(handle, &finddata);
		if (result == -1)
			break;
	}
	_findclose(handle);

	if (fileNames == NULL) {
		printf("��ȣ�� ������ ������ ������ �־��ּ���!\n(��ġ : (���α׷��� �ִ� ������ input ���� ���ο�)\n");
		return NULL;
	}
	else if (fileNames->nextFile == NULL)
		return fileNames->filename;
	else {
		printf("������ ������ �߰ߵǾ����ϴ�.\n");
		char userInput[50];
		fileName* root = fileNames;
		while (true) {
			printf("  ���� : %s �� �³���? (y/n)\n", root->filename);
			scanf_s("%s", userInput,50);
			if (strcmp(userInput, "y") == 0) {
				printf("  ���� : %s ���� �����մϴ�.\n", root->filename);
				return root->filename;
			}
			else if (strcmp(userInput, "n") == 0) {
				root = root->nextFile;
				if (root == NULL) {
					printf("�� �̻� ã�� �� �ִ� ������ �����ϴ�!\n");
					return NULL;
				}
			}
			else 
				printf("�߸��� �Է� : �ٽ� �Է����ּ���.\n");
		}
	}
}

// �׽�Ʈ��
void readFile() {
	char *filename = getUserInputFile();
	if (filename == NULL) {
		// non file
	}
	else {
		FILE* file;
		char filePath[500];
		strcpy_s(filePath, USER_INPUT_DIR_SHORT);
		strcat_s(filePath, filename);
		printf("�����ڵ� : %d\n",fopen_s(&file, filePath, "rb"));

		char fileContent[5000];
		int readLen, index;

		const char* keyTitle = "encryptedKey";
		int keyIdx = 0;

		// �׽�Ʈ�� - Ű ã�� �κ�
		int isFinded = false;
		while (!feof(file)) {
			fread(fileContent, sizeof(char), 5000, file);
			for (index = 0; index < 5000; index++) {
				while (true) {
					if (keyTitle[keyIdx] == '\0') {
						isFinded = true;
						printf("%c%c%c\n", fileContent[index-4], fileContent[index - 3], fileContent[index - 2]);
						keyIdx = 0;
						break;
					}
					else if (fileContent[index] == keyTitle[keyIdx]) {
						index++;
						keyIdx++;
						if (index == 5000)
							break;
					}
					else {
						keyIdx == 0;
						break;
					}
				}
				if (isFinded == true)
					break;
			}
			printf("�߰�? : %c\n",(isFinded?'y':'n'));
		}

		fclose(file);
	}
}