#pragma once
#include <Windows.h>

struct KeyData {
	int spinCount;
	char* Salt;
	char* EncryptedVerifierHashInput;
	char* EncryptedVerifierHashValue;
};

struct fileData {
	char** keyData;
	char filename[500];
	fileData* nextFile;
};

KeyData* getKeyData();
