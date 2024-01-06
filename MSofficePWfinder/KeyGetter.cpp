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
		printf("암호를 해제할 파일을 폴더에 넣어주세요!\n(위치 : (프로그램이 있는 폴더의 input 폴더 내부에)\n");
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
		printf("암호를 해제할 파일을 폴더에 넣어주세요!\n(위치 : (프로그램이 있는 폴더의 input 폴더 내부에)\n");
		return NULL;
	}
	else if (fileNames->nextFile == NULL)
		return fileNames->filename;
	else {
		printf("파일이 여러개 발견되었습니다.\n");
		char userInput[50];
		fileName* root = fileNames;
		while (true) {
			printf("  파일 : %s 이 맞나요? (y/n)\n", root->filename);
			scanf_s("%s", userInput,50);
			if (strcmp(userInput, "y") == 0) {
				printf("  파일 : %s 으로 진행합니다.\n", root->filename);
				return root->filename;
			}
			else if (strcmp(userInput, "n") == 0) {
				root = root->nextFile;
				if (root == NULL) {
					printf("더 이상 찾을 수 있는 파일이 없습니다!\n");
					return NULL;
				}
			}
			else 
				printf("잘못된 입력 : 다시 입력해주세요.\n");
		}
	}
}

// 테스트중
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
		printf("오류코드 : %d\n",fopen_s(&file, filePath, "rb"));

		char fileContent[5000];
		int readLen, index;

		const char* keyTitle = "encryptedKey";
		int keyIdx = 0;

		// 테스트중 - 키 찾는 부분
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
			printf("발견? : %c\n",(isFinded?'y':'n'));
		}

		fclose(file);
	}
}