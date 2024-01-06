#pragma once
#include <Windows.h>

struct KeyData {
	int isValid;
	PWCHAR Salt;
	PWCHAR EncryptedVerifierHashInput;
	PWCHAR EncryptedVerifierHashValue;
};

struct fileName {
	char filename[500];
	fileName* nextFile;
};

#define USER_INPUT_FILE "input.txt"
#define USER_INPUT_DIR_SHORT "..\\Debug\\input\\"//".\\input\\"

KeyData* get();

void saveFileName(fileName** root, char* filename);

char* getUserInputFile();

void readFile();