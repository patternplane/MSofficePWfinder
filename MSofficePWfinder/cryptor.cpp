#include <Windows.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <stdio.h>
#include "base64.h"
#include "StrGetter.h"

#include "KeyGetter.h"
#include "KeyData.h"

// 코드 참고 출처 : https://codetronik.tistory.com/93

#define MAX_FILE_SIZE 999999

int hasKeyData = 0;
KeyData* keydata;
void setKeyData(KeyData* kd) {
	if (kd != NULL) {
		keydata = kd;
		hasKeyData = 1;
	}
	else {
		printf("Wrong KeyData!\n");
	}
}

// 문서 복호화
void Decrypt2(
	IN PBYTE pbySalt, // salt
	IN PBYTE pbyKey, // 복호화용 대칭키
	IN PBYTE pbyEncryptedContent, // 암호화 된 컨텐츠
	OUT PBYTE pbyDecryptedContent, // 복호화 된 컨텐츠
	IN int cbDecryptLength // 복호화 된 문서 길이   
)
{

	DWORD dwBlockSize = 4096; // 블록 길이
	DWORD dwRead = 0; // 총 복호화 한 길이
	int n = (cbDecryptLength + dwBlockSize - 1) / dwBlockSize; // 복호화 횟수
	for (int i = 0; i < n; i++)
	{
		DWORD dwLen = 0;
		// 한 블록당 복호화 할 길이를 구한다.
		if (i < n - 1)
		{
			dwLen = dwBlockSize;
		}
		else
		{
			dwLen = cbDecryptLength % dwBlockSize;
		}

		// iv + blockkey
		BYTE temp[20] = { 0, };
		// 블록키는 1씩 증가한다.
		BYTE byBlockKey[4] = { 0, };
		memcpy(byBlockKey, &i, sizeof(int));

		memcpy(temp, pbySalt, 16); // 16바이트를 salt로 채운다.
		memcpy(temp + 16, byBlockKey, 4); // 블록키를 iv뒤에 4바이트 이어붙임

		// 64바이트 iv hash생성
		BYTE ivhash[64] = { 0, };
		SHA512_CTX ctx;
		SHA512_Init(&ctx);
		SHA512_Update(&ctx, temp, 20);
		SHA512_Final(ivhash, &ctx);

		BYTE iv[32] = { 0, }; // iv는 32바이트이다.
		AES_KEY akey;
		memcpy(iv, ivhash, 32); // IV 해시에서 32바이트만큼 복사한다.
		memset(&akey, 0, sizeof(AES_KEY));

		// iv와 대칭키를 설정한다.
		if (AES_set_decrypt_key(pbyKey, 256, &akey) < 0)
		{
			printf("error\n");
			return;
		}

		AES_cbc_encrypt(pbyEncryptedContent + (i * dwBlockSize), (unsigned char*)pbyDecryptedContent + dwRead, dwLen, &akey, iv, AES_DECRYPT);
		dwRead += dwLen;
	}
}

// 키 복호화
void Decrypt(
	IN PBYTE pbySalt, // salt
	IN PBYTE pbyKey, // 복호화용 대칭키
	IN PBYTE pbyEncryptedContent, // 암호화 된 컨텐츠
	OUT PBYTE pbyDecryptedContent, // 복호화 된 컨텐츠
	IN int cbDecryptLength // 복호화 된 검증값 길이
)
{
	BYTE iv[32] = { 0, }; // iv는 32바이트이다.
	AES_KEY akey;
	memcpy(iv, pbySalt, 16); // 16바이트를 salt로 채운다.
	memset(&iv[16], 0, 16); // 16바이트 이후를 0으로 채운다.
	memset(&akey, 0, sizeof(AES_KEY));

	// iv와 대칭키를 설정한다.
	if (AES_set_decrypt_key(pbyKey, 256, &akey) < 0)
	{
		printf("error\n");
		return;
	}
	AES_cbc_encrypt(pbyEncryptedContent, (unsigned char*)pbyDecryptedContent, cbDecryptLength, &akey, iv, AES_DECRYPT);
}

// 패스워드 해시
void GenPasswordHash(
	OUT PBYTE pbyHash,
	IN PWCHAR pszPassword,
	IN PBYTE pbySalt)
{
	// spin count를 입력해야하기 때문에 UINT형을 사용한다.
	UINT32 uiBuffer[(SHA512_DIGEST_LENGTH / sizeof(UINT32)) + 1] = { 0, };
	int len = wcslen(pszPassword) * 2; // 패스워드는 유니코드로 처리한다.
	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, pbySalt, 16);
	SHA512_Update(&ctx, pszPassword, len);
	SHA512_Final((BYTE*)&uiBuffer[1], &ctx);

	// spin_count = 100000
	for (int i = 0; i < keydata->spinCount; i++)
	{
		*uiBuffer = i; // spin count
		SHA512_Init(&ctx);
		SHA512_Update(&ctx, &uiBuffer, SHA512_DIGEST_LENGTH + 4 /*spin count buffer length */);
		SHA512_Final((BYTE*)&uiBuffer[1], &ctx);
	}
	memcpy(pbyHash, (BYTE*)&uiBuffer[1], SHA512_DIGEST_LENGTH);
}

void GenAgileEncryptionKey(
	IN PBYTE pbyHash, // password 해시
	IN PBYTE pbyBlockKey, // 블록키
	OUT PBYTE pbyEncKey // 생성된 대칭키
)
{
	BYTE byHashWithKey[SHA512_DIGEST_LENGTH + 8] = { 0, };

	// password 해시 복사
	memcpy(byHashWithKey, pbyHash, SHA512_DIGEST_LENGTH);

	// 해시 뒤에 블록키 붙임
	memcpy(&byHashWithKey[SHA512_DIGEST_LENGTH], pbyBlockKey, 8);

	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, byHashWithKey, SHA512_DIGEST_LENGTH + 8);
	SHA512_Final(pbyEncKey, &ctx);
}

   /*----------------------------
            패스워드 검증
   -----------------------------*/

int isPasswordChecked = 0;
BYTE byPwHash[64] = { 0, };
BYTE bySalt[16] = { 1, };

PWCHAR checkCorrectPassword(int threadIdx) {
	if (!hasKeyData) {
		printf("KeyData has not been assigned!\n");
		return NULL;
	}

	// 블록 키 (고정값)
	BYTE byHashInputBlockKey[] = { 0xfe, 0xa7, 0xd2, 0x76, 0x3b, 0x4b, 0x9e, 0x79 };
	BYTE byHashValueBlockKey[] = { 0xd7, 0xaa, 0x0f, 0x6d, 0x30, 0x61, 0x34, 0x4e };

	// decode64 된 데이터
	bySalt;
	BYTE byEncryptedVerifierHashInput[16] = { 0, };
	BYTE byEncryptedVerifierHashValue[64] = { 0, };

	base64_decode(keydata->Salt, bySalt, 16);
	base64_decode(keydata->EncryptedVerifierHashInput, byEncryptedVerifierHashInput, 16);
	base64_decode(keydata->EncryptedVerifierHashValue, byEncryptedVerifierHashValue, 64);

	// 암호화 데이터를 복호화하는 대칭키
	BYTE byHashInputKey[SHA512_DIGEST_LENGTH] = { 0, };
	BYTE byHashValueKey[SHA512_DIGEST_LENGTH] = { 0, };

	// 패스워드 해시를 담을 변수
	byPwHash;

	// 복호화 데이터를 담을 변수
	BYTE byDecryptedVerifierHashInput[16] = { 0, };
	BYTE byDecryptedVerifierHashValue[64] = { 0, };

	// 복호화된 검증용 해시 입력을 해시한 결과를 담을 변수
	BYTE byFinalHash[SHA512_DIGEST_LENGTH] = { 0, };
	SHA512_CTX ctx;

	// 비밀번호 테스트값
	WCHAR password[50];

	// 최종 해시 비교값 (0 = 옳은 비밀번호)
	int nCmp;

	for (int i = 0; ; i++) {
		getStr(password, threadIdx);
		nextStr(threadIdx);
		if (i % 500 == 0)
			wprintf(L"loading : %s\n", password);

		// 패스워드 해시를 구함
		// ((wchar_t*)(int*)Password)
		GenPasswordHash(byPwHash, password, bySalt);

		// 대칭키 생성 (with password hash)
		GenAgileEncryptionKey(byPwHash, byHashInputBlockKey, byHashInputKey);
		GenAgileEncryptionKey(byPwHash, byHashValueBlockKey, byHashValueKey);

		// 복호화 
		Decrypt(bySalt, byHashInputKey, byEncryptedVerifierHashInput, byDecryptedVerifierHashInput, 16);
		Decrypt(bySalt, byHashValueKey, byEncryptedVerifierHashValue, byDecryptedVerifierHashValue, 64);

		// 복호화 된 검증용 해시 입력을 해시한다.
		SHA512_Init(&ctx);
		SHA512_Update(&ctx, byDecryptedVerifierHashInput, 16);
		SHA512_Final(byFinalHash, &ctx);

		// 해시 비교한다.
		nCmp = memcmp(byFinalHash, byDecryptedVerifierHashValue, SHA512_DIGEST_LENGTH);

		if (0 == nCmp) {
			wprintf(L"%s : FINDED!\n", password);
			break;
		}
	}

	if (nCmp == 0) {
		isPasswordChecked = 1;
		return password;
	}
	else
		return NULL;
}


    /*----------------------------
            문서 복호화
    -----------------------------*/
void fileEncryptor() {

	if (isPasswordChecked == 0) {
		printf("password non checked!\n");
		return;
	}

	// 블록 키 (고정값)
	BYTE byKeyBlockKey[] = { 0x14, 0x6e, 0x0b, 0xe7, 0xab, 0xac, 0xd0, 0xd6 };

	BYTE* byEncryptedFileBuffer = NULL;
	BYTE* byDecryptedFileBuffer = NULL;

	byEncryptedFileBuffer = (BYTE*)malloc(MAX_FILE_SIZE);
	byDecryptedFileBuffer = (BYTE*)malloc(MAX_FILE_SIZE);

	UINT64* pullDecryptLength = (UINT64*)malloc(sizeof(UINT64));

	// EncryptPackage 파일 오픈
	HANDLE hFile = CreateFile(L"D:\\EncryptedPackage2", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);

	DWORD dwRead = 0;
	ReadFile(hFile, byEncryptedFileBuffer, MAX_FILE_SIZE, &dwRead, NULL);

	// 앞 8바이트에서 복호화 길이를 구한다.
	memcpy(pullDecryptLength, byEncryptedFileBuffer, 8);

	BYTE bySaltValue[16] = { 0, };
	BYTE byEncryptedKeyValue[32] = { 0, };

	base64_decode(SaltValue, bySaltValue, 16);
	base64_decode(EncryptedKeyValue, byEncryptedKeyValue, 32);

	// 대칭키 생성 (with password hash)
	BYTE byKeyValueKey[SHA512_DIGEST_LENGTH] = { 0, };
	GenAgileEncryptionKey(byPwHash, byKeyBlockKey, byKeyValueKey);

	// 대칭키를 복호화한다.
	BYTE byDecryptedKeyValue[32] = { 0, };
	Decrypt(bySalt, byKeyValueKey, byEncryptedKeyValue, byDecryptedKeyValue, 32);

	HANDLE hSaveFile = CreateFile(L"D:\\Decrypt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	// 문서를 복호화한다.
	Decrypt2(bySaltValue, byDecryptedKeyValue, &byEncryptedFileBuffer[8], byDecryptedFileBuffer, *pullDecryptLength);


	DWORD dwWritten = 0;
	WriteFile(hSaveFile, byDecryptedFileBuffer, *pullDecryptLength, &dwWritten, NULL);

	free(pullDecryptLength);
	getchar();
}