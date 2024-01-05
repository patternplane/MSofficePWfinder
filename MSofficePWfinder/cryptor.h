#pragma once
#include <Windows.h>

// 참고블로그 : https://codetronik.tistory.com/93

void Decrypt2(
	IN PBYTE pbySalt, // salt
	IN PBYTE pbyKey, // 복호화용 대칭키
	IN PBYTE pbyEncryptedContent, // 암호화 된 컨텐츠
	OUT PBYTE pbyDecryptedContent, // 복호화 된 컨텐츠
	IN int cbDecryptLength // 복호화 된 문서 길이   
);

void Decrypt(
	IN PBYTE pbySalt, // salt
	IN PBYTE pbyKey, // 복호화용 대칭키
	IN PBYTE pbyEncryptedContent, // 암호화 된 컨텐츠
	OUT PBYTE pbyDecryptedContent, // 복호화 된 컨텐츠
	IN int cbDecryptLength // 복호화 된 검증값 길이
);

void GenPasswordHash(
	OUT PBYTE pbyHash,
	IN PWCHAR pszPassword,
	IN PBYTE pbySalt
);

void GenAgileEncryptionKey(
	IN PBYTE pbyHash, // password 해시
	IN PBYTE pbyBlockKey, // 블록키
	OUT PBYTE pbyEncKey // 생성된 대칭키
);

/*----------------------------
            패스워드 검증
   -----------------------------*/
PWCHAR checkCorrectPassword(int threadIdx);

/*----------------------------
            문서 복호화
	-----------------------------*/
void fileEncryptor();