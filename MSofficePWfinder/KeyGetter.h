#pragma once
#include <Windows.h>

struct KeyData {
	int spinCount;
	PWCHAR Salt;
	PWCHAR EncryptedVerifierHashInput;
	PWCHAR EncryptedVerifierHashValue;
};

struct fileData {
	WCHAR** keyData;
	char filename[500];
	fileData* nextFile;
};

#define USER_INPUT_FILE "input.txt"
#define USER_INPUT_DIR_SHORT "..\\Debug\\input\\"//".\\input\\"

KeyData* getKeyData();
