#include "StrGetter2.h"

currentStep step;
int needInit;
int firstIdx;
int secondIdx;
WCHAR firstChar;
WCHAR secondChar;
const WCHAR START_CHAR = ' ';
const WCHAR END_CHAR = '~';

DictionData* passwords;
int currentWord;

WCHAR origin[CHAR_LEN];
WCHAR origin_CR[CHAR_LEN];
int originLen;

int setWord() {
	int result = -(currentWord == passwords->amount - 1);
	if (currentWord == passwords->amount - 1)
		currentWord = 0;
	else
		currentWord++;

	for (int i = 0; ; i++) {
		origin[i] = passwords->diction[currentWord][i];
		origin_CR[i] = ('a' <= passwords->diction[currentWord][i] && passwords->diction[currentWord][i] <= 'z'
			? passwords->diction[currentWord][i] - 'a' + 'A'
			: ('A' <= passwords->diction[currentWord][i] && passwords->diction[currentWord][i] <= 'Z'
				? passwords->diction[currentWord][i] - 'A' + 'a'
				: passwords->diction[currentWord][i]));
		if (passwords->diction[currentWord][i] == '\0') {
			originLen = i;
			break;
		}
	}

	return result;
}

void strInit(DictionData* initStrs) {
	step = original;
	needInit = true;
	passwords = initStrs;

	if (initStrs->amount == 0)
		return;

	currentWord = -1;

	setWord();
}

void getQuotaData(quotaData* destination) {
	if (passwords->amount == 0) {
		destination->amount = 0;
		return;
	}

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
			if (-1 == setWord()) {
				step = d1;
				needInit = true;
			}
		}

		// 1개 삭제 - 테스트 완료
		else if (step == d1) {
			if (needInit) {
				firstIdx = 0;
				needInit = false;
			}

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
				if (-1 == setWord())
					step = a1;
				needInit = true;
			}
		}

		// 1개 추가 - 테스트 완료
		else if (step == a1) {
			if (needInit) {
				firstIdx = 0;
				firstChar = START_CHAR;
				needInit = false;
			}

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
					if (-1 == setWord())
						step = c1;
					needInit = true;
				}
				else
					firstIdx++;
			}
			else
				firstChar++;
		}

		// 1개 변경 - 테스트 완료
		else if (step == c1) {
			if (needInit) {
				firstIdx = 0;
				firstChar = START_CHAR;
				needInit = false;
			}

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
					if (-1 == setWord())
						step = d2;
					needInit = true;
				}
			}
			else
				firstChar++;
		}

		// 2개 삭제 - 테스트 완료
		else if (step == d2) {
			if (needInit) {
				firstIdx = 0;
				secondIdx = firstIdx + 1;
				needInit = false;
			}

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
					if (-1 == setWord())
						step = a2;
					needInit = true;
				}
			}
		}

		// 2개 추가 - 테스트 완료
		else if (step == a2) {
			if (needInit) {
				firstIdx = 0;
				secondIdx = firstIdx;
				firstChar = START_CHAR;
				secondChar = START_CHAR;
				needInit = false;
			}

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
							if (-1 == setWord())
								step = c2;
							needInit = true;
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
			if (needInit) {
				firstIdx = 0;
				secondIdx = firstIdx + 1;
				firstChar = START_CHAR;
				secondChar = START_CHAR;
				needInit = false;
			}
			
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
							if (-1 == setWord())
								step = d1a1;
							needInit = true;
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
			if (needInit) {
				firstIdx = 0;
				secondIdx = 0;
				firstChar = START_CHAR;
				needInit = false;
			}

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
						if (-1 == setWord())
							step = d1c1;
						needInit = true;
					}
				}
			}
			else
				firstChar++;
		}

		// 1삭제 1변경 - 테스트 완료
		else if (step == d1c1) {
			if (needInit) {
				firstIdx = 0;
				secondIdx = 0;
				firstChar = START_CHAR;
				needInit = false;
			}
			
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
							if (-1 == setWord())
								step = a1c1;
							needInit = true;
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
			if (needInit) {
				firstIdx = 0;
				secondIdx = 1;
				firstChar = START_CHAR;
				secondChar = START_CHAR;
				needInit = false;
			}

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
								if (-1 == setWord())
									step = end;
								needInit = true;
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