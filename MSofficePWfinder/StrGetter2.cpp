#include <windows.h>
#include <stdio.h>
#include "StrGetter2.h"

currentStep step;
int firstIdx;
int secondIdx;
char firstChar;
char secondChar;
const char START_CHAR = ' ';
const char END_CHAR = '~';

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

void getQuotaData(quotaData* destination) {

	int cnt = 0;

	while (true) {
		if (cnt == QUOTA_AMOUN
			|| step == end)
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
				firstChar = START_CHAR;
			}
		}

		// 1개 추가 - 테스트 완료
		else if (step == a1) {
			for (int j = 0, oi = 0; ; oi++) {
				if (oi == firstIdx) {
					destination->quota[cnt][j] = firstChar;
					destination->quota[cnt + 1][j] = firstChar;
					j++;
				}
				destination->quota[cnt][j] = origin[oi];
				destination->quota[cnt + 1][j++] = origin_CR[oi];
				if (origin[oi] == '\0')
					break;;
			}

			cnt += 2;
			if (firstChar == END_CHAR) {
				firstChar = START_CHAR;
				if (firstIdx == originLen) {
					step = c1;
					firstIdx = 0;
					firstChar = START_CHAR;
				}
				else
					firstIdx++;
			}
			else
				firstChar++;
		}

		// 1개 변경 - 테스트 완료
		else if (step == c1) {
			for (int i = 0; i < originLen + 1; i++) {
				destination->quota[cnt][i] = origin[i];
				destination->quota[cnt + 1][i] = origin_CR[i];
			}
			destination->quota[cnt][firstIdx] = firstChar;
			destination->quota[cnt + 1][firstIdx] = firstChar;

			cnt += 2;
			if (firstChar == END_CHAR) {
				firstChar = START_CHAR;
				firstIdx++;
				if (firstIdx == originLen) {
					step = d2;
					firstIdx = 0;
					secondIdx = firstIdx + 1;
				}
			}
			else
				firstChar++;
		}

		// 2개 삭제 - 테스트 완료
		else if (step == d2) {
			for (int k = 0, oi = 0; ; oi++) {
				if (origin[oi] == '\0'
					|| (oi != firstIdx
					&& oi != secondIdx)) {
					destination->quota[cnt][k] = origin[oi];
					destination->quota[cnt + 1][k++] = origin_CR[oi];
				}
				if (origin[oi] == '\0')
					break;
			}

			cnt += 2;
			secondIdx++;
			if (secondIdx == originLen) {
				firstIdx++;
				secondIdx = firstIdx + 1;
				if (firstIdx == originLen - 1) {
					step = a2;
					firstIdx = 0;
					secondIdx = firstIdx;
					firstChar = START_CHAR;
					secondChar = START_CHAR;
				}
			}
		}

		// 2개 추가 - 테스트 완료
		else if (step == a2) {
			for (int k = 0, oi = 0; ; oi++) {
				if (oi == firstIdx)
					k++;
				if (oi == secondIdx)
					k++;
				destination->quota[cnt][k] = origin[oi];
				destination->quota[cnt + 1][k++] = origin_CR[oi];
				if (origin[oi] == '\0')
					break;;
			}
			destination->quota[cnt][firstIdx] = firstChar;
			destination->quota[cnt][secondIdx + 1] = secondChar;
			destination->quota[cnt + 1][firstIdx] = firstChar;
			destination->quota[cnt + 1][secondIdx + 1] = secondChar;

			cnt += 2;
			if (secondChar == END_CHAR) {
				if (firstChar == END_CHAR) {
					firstChar = START_CHAR;
					secondChar = START_CHAR;
					secondIdx++;
					if (secondIdx == originLen + 1) {
						firstIdx++;
						secondIdx = firstIdx;
						if (firstIdx == originLen + 1) {
							step = c2;
							firstIdx = 0;
							secondIdx = firstIdx + 1;
							firstChar = START_CHAR;
							secondChar = START_CHAR;
						}
					}
				}
				else {
					firstChar++;
					secondChar = START_CHAR;
				}
			}
			else
				secondChar++;
		}

		// 2개 변경 - 테스트 완료
		else if (step == c2) {
			for (int k = 0; k < originLen + 1; k++) {
				destination->quota[cnt][k] = origin[k];
				destination->quota[cnt + 1][k] = origin_CR[k];
			}
			destination->quota[cnt][firstIdx] = firstChar;
			destination->quota[cnt][secondIdx] = secondChar;
			destination->quota[cnt + 1][firstIdx] = firstChar;
			destination->quota[cnt + 1][secondIdx] = secondChar;

			cnt += 2;
			if (secondChar == END_CHAR) {
				if (firstChar == END_CHAR) {
					firstChar = START_CHAR;
					secondChar = START_CHAR;
					secondIdx++;
					if (secondIdx == originLen) {
						firstIdx++;
						secondIdx = firstIdx + 1;
						if (firstIdx == originLen - 1) {
							step = d1a1;
							firstIdx = 0;
							secondIdx = 0;
							firstChar = START_CHAR;
						}
					}
				}
				else {
					firstChar++;
					secondChar = START_CHAR;
				}
			}
			else
				secondChar++;
		}

		// 1삭제 1추가 - 테스트 완료
		else if (step == d1a1) {
			for (int k = 0, oi = 0; ; oi++) {
				if (k == secondIdx)
					k++;
				if (oi != firstIdx) {
					destination->quota[cnt][k] = origin[oi];
					destination->quota[cnt + 1][k++] = origin_CR[oi];
				}
				if (origin[oi] == '\0')
					break;
			}
			destination->quota[cnt][secondIdx] = firstChar;
			destination->quota[cnt + 1][secondIdx] = firstChar;

			cnt += 2;
			if (firstChar == END_CHAR) {
				firstChar = START_CHAR;
				secondIdx++;
				if (secondIdx == originLen) {
					firstIdx++;
					secondIdx = 0;
					if (firstIdx == originLen) {
						step = d1c1;
						firstIdx = 0;
						secondIdx = 0;
						firstChar = START_CHAR;
					}
				}
			}
			else
				firstChar++;
		}

		// 1삭제 1변경 - 테스트 완료
		else if (step == d1c1) {
			for (int k = 0, oi = 0; ; oi++) {
				if (oi != firstIdx) {
					destination->quota[cnt][k] = origin[oi];
					destination->quota[cnt + 1][k++] = origin_CR[oi];
				}
				if (origin[oi] == '\0')
					break;
			}
			destination->quota[cnt][secondIdx] = firstChar;
			destination->quota[cnt + 1][secondIdx] = firstChar;

			cnt += 2;
			if (firstChar == END_CHAR) {
				firstChar = START_CHAR;
				do {
					secondIdx++;
					if (secondIdx == originLen - 1) {
						firstIdx++;
						secondIdx = 0;
						if (firstIdx == originLen) {
							step = a1c1;
							firstIdx = 0;
							secondIdx = 1;
							firstChar = START_CHAR;
							secondChar = START_CHAR;
						}
					}
				} while (step == d1c1
					&& secondIdx == firstIdx - 1);
			}
			else
				firstChar++;
		}

		// 1추가 1변경 - 테스트 완료
		else if (step == a1c1) {
			for (int j = 0, oi = 0; ; oi++) {
				if (oi == firstIdx)
					j++;
				destination->quota[cnt][j] = origin[oi];
				destination->quota[cnt + 1][j++] = origin_CR[oi];
				if (origin[oi] == '\0')
					break;;
			}
			destination->quota[cnt][firstIdx] = firstChar;
			destination->quota[cnt][secondIdx] = secondChar;
			destination->quota[cnt + 1][firstIdx] = firstChar;
			destination->quota[cnt + 1][secondIdx] = secondChar;

			cnt += 2;
			if (secondChar == END_CHAR) {
				if (firstChar == END_CHAR) {
					firstChar = START_CHAR;
					secondChar = START_CHAR;
					do {
						secondIdx++;
						if (secondIdx == originLen + 1) {
							firstIdx++;
							secondIdx = 0;
							if (firstIdx == originLen + 1) {
								step = end;
							}
						}
					} while (step == a1c1
						&& !(firstIdx != secondIdx
							&& (firstIdx - 1 != secondIdx)));
				}
				else {
					firstChar++;
					secondChar = START_CHAR;
				}
			}
			else
				secondChar++;
		}
	}

	destination->amount = cnt;
}