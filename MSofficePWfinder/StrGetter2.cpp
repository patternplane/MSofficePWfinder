#include <windows.h>
#include <stdio.h>
#include "StrGetter2.h"

currentStep step;
int firstIdx;
int secondIdx;
char firstChar;
char secondChar;

char* origin;
char* origin_CR;
int originLen;

void strInit(const char* initStr) {
	step = original;

	for (int i = 0; ; i++)
		if (initStr[i] == '\0') {
			originLen = i;
			break;
		}
	origin = (char*)malloc((originLen + 1) * sizeof(char));
	origin_CR = (char*)malloc((originLen + 1) * sizeof(char));
	for (int i = 0; i < originLen + 1; i++) {
		origin[i] = initStr[i];
		origin_CR[i] = ('a' <= initStr[i] && initStr[i] <= 'z'
			? initStr[i] - 'a' + 'A'
			: ('A' <= initStr[i] && initStr[i] <= 'Z'
				? initStr[i] - 'A' + 'a'
				: initStr[i]));
	}
}

void test(quotaData* destination) {

	int cnt = 0;

	while (true) {
		if (cnt == QUOTA_AMOUN
			|| step == a1) // step == end
			break;

		// 기본값 - 테스트 완료
		if (step == original) {
			for (int i = 0; i < originLen + 1; i++) {
				destination->quota[cnt][i] = origin[i];
				destination->quota[cnt + 1][i] = origin_CR[i];
			}

			cnt += 2;
			step = d1;
			firstIdx = 0;
		}

		// 1개 삭제 - 테스트 완료
		else if (step == d1) {
			for (int j = 0, oi = 0; ; oi++) {
				if (oi != firstIdx) {
					destination->quota[cnt][j] = origin[oi];
					destination->quota[cnt + 1][j++] = origin_CR[oi];
				}
				if (origin[oi] == '\0')
					break;
			}
			cnt += 2;
			firstIdx++;
			if (firstIdx == originLen) {
				step = a1;
				firstIdx = 0;
				firstChar = ' ';
			}
		}

		// QUOTA_AMOUN 가 작을 경우,
		// 다시 함수를 호출했을 때 다음것이 잘 연결되어 출력되는지도
		// 테스트 필요
		// -> 모두 완료된 후 다시 호출하면 cnt가 정상적으로 0으로 나오는지도.
	}

	destination->amount = cnt;


	// 스레드별 한번에 100개씩 주는 것도 좋을듯.
	// 100개 이상씩. 병목현상 없도록.

	// CAPS LOCK 동시처리 유의.


	// 1추
	//for (int i = 0; i <= strLen; i++) {
	//	for (int j = 0, oi = 0; ; oi++) {
	//		if (oi == i)
	//			j++;
	//		str[j++] = ori[oi];
	//		if (ori[oi] == '\0')
	//			break;;
	//	}
	//	// start-end 처리
	//	for (char j = '0'; j < '5'; j++) {
	//		str[i] = j;
	//		// str 처리
	//	}
	//}

	// 1변
	//for (int i = 0; i < strLen; i++) {
	//	for (int i = 0; i < strLen + 1; i++)
	//		str[i] = ori[i];
	//	// start-end 처리
	//	for (char j = '0'; j < '5'; j++) {
	//		str[i] = j;
	//		// str 처리
	//	}
	//}

	// 2삭
	//for (int i = 0; i < strLen; i++)
	//	for (int j = i + 1; j < strLen; j++) {
	//		for (int k = 0, oi = 0; ; oi++) {
	//			if (oi != i
	//				&& oi != j) {
	//				str[k++] = ori[oi];
	//			}
	//			if (ori[oi] == '\0')
	//				break;
	//		}
	//		// str 처리
	//	}

	// 2추
	//for (int i = 0; i <= strLen; i++)
	//	for (int j = i; j <= strLen; j++) {
	//		for (int k = 0, oi = 0; ; oi++) {
	//			if (oi == i)
	//				k++;
	//			if (oi == j)
	//				k++;
	//			str[k++] = ori[oi];
	//			if (ori[oi] == '\0')
	//				break;;
	//		}
	//		// start-end 처리
	//		for (char m = '0'; m < '5'; m++) {
	//			str[i] = m;
	//			// start-end 처리
	//			for (char n = '0'; n < '5'; n++) {
	//				str[j + 1] = n;
	//				// str 처리
	//			}
	//		}
	//	}

	// 2변
	//for (int i = 0; i < strLen; i++)
	//	for (int j = i + 1; j < strLen; j++) {
	//		for (int k = 0; k < strLen + 1; k++)
	//			str[k] = ori[k];
	//		// start-end 처리
	//		for (char m = '0'; m < '5'; m++) {
	//			str[i] = m;
	//			// start-end 처리
	//			for (char n = '0'; n < '5'; n++) {
	//				str[j] = n;
	//				// str 처리
	//			}
	//		}
	//	}
		
	// 1삭 1변
	//for (int i = 0; i < strLen; i++) {
	//	for (int j = 0; j < strLen - 1; j++) {
	//		for (int k = 0, oi = 0; ; oi++) {
	//			if (oi != i)
	//				str[k++] = ori[oi];
	//			if (ori[oi] == '\0')
	//				break;
	//		}
	//		// start-end 처리
	//		for (char m = '0'; m < '5'; m++) {
	//			str[j] = m;
	//			// str처리
	//		}
	//	}
	//} 

	// 1삭 1추
	//for (int i = 0; i < strLen; i++) {
	//	for (int j = 0; j < strLen; j++) {
	//		for (int k = 0, oi = 0; ; oi++) {
	//			if (k == j)
	//				k++;
	//			if (oi != i)
	//				str[k++] = ori[oi];
	//			if (ori[oi] == '\0')
	//				break;
	//		}
	//		// start-end 처리
	//		for (char m = '0'; m < '5'; m++) {
	//			str[j] = m;
	//			// str처리
	//		}
	//	}
	//}
	
	// 1변 1추
	//for (int i = 0; i <= strLen; i++) {
	//	for (int j = 0; j < strLen + 1; j++) {
	//		if (i != j
	//			&& (i - 1 != j)) {
	//			for (int j = 0, oi = 0; ; oi++) {
	//				if (oi == i)
	//					j++;
	//				str[j++] = ori[oi];
	//				if (ori[oi] == '\0')
	//					break;;
	//			}
	//			// start-end 처리
	//			for (char m = '0'; m < '5'; m++) {
	//				str[i] = m;
	//				// start-end 처리
	//				for (char n = '0'; n < '5'; n++) {
	//					str[j] = n;
	//					// str 처리
	//				}
	//			}
	//		}
	//	}
	//}
}