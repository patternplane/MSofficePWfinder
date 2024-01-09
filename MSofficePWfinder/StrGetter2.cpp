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

		// �⺻�� - �׽�Ʈ �Ϸ�
		if (step == original) {
			for (int i = 0; i < originLen + 1; i++) {
				destination->quota[cnt][i] = origin[i];
				destination->quota[cnt + 1][i] = origin_CR[i];
			}

			cnt += 2;
			step = d1;
			firstIdx = 0;
		}

		// 1�� ���� - �׽�Ʈ �Ϸ�
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

		// QUOTA_AMOUN �� ���� ���,
		// �ٽ� �Լ��� ȣ������ �� �������� �� ����Ǿ� ��µǴ�����
		// �׽�Ʈ �ʿ�
		// -> ��� �Ϸ�� �� �ٽ� ȣ���ϸ� cnt�� ���������� 0���� ����������.
	}

	destination->amount = cnt;


	// �����庰 �ѹ��� 100���� �ִ� �͵� ������.
	// 100�� �̻�. �������� ������.

	// CAPS LOCK ����ó�� ����.


	// 1��
	//for (int i = 0; i <= strLen; i++) {
	//	for (int j = 0, oi = 0; ; oi++) {
	//		if (oi == i)
	//			j++;
	//		str[j++] = ori[oi];
	//		if (ori[oi] == '\0')
	//			break;;
	//	}
	//	// start-end ó��
	//	for (char j = '0'; j < '5'; j++) {
	//		str[i] = j;
	//		// str ó��
	//	}
	//}

	// 1��
	//for (int i = 0; i < strLen; i++) {
	//	for (int i = 0; i < strLen + 1; i++)
	//		str[i] = ori[i];
	//	// start-end ó��
	//	for (char j = '0'; j < '5'; j++) {
	//		str[i] = j;
	//		// str ó��
	//	}
	//}

	// 2��
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
	//		// str ó��
	//	}

	// 2��
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
	//		// start-end ó��
	//		for (char m = '0'; m < '5'; m++) {
	//			str[i] = m;
	//			// start-end ó��
	//			for (char n = '0'; n < '5'; n++) {
	//				str[j + 1] = n;
	//				// str ó��
	//			}
	//		}
	//	}

	// 2��
	//for (int i = 0; i < strLen; i++)
	//	for (int j = i + 1; j < strLen; j++) {
	//		for (int k = 0; k < strLen + 1; k++)
	//			str[k] = ori[k];
	//		// start-end ó��
	//		for (char m = '0'; m < '5'; m++) {
	//			str[i] = m;
	//			// start-end ó��
	//			for (char n = '0'; n < '5'; n++) {
	//				str[j] = n;
	//				// str ó��
	//			}
	//		}
	//	}
		
	// 1�� 1��
	//for (int i = 0; i < strLen; i++) {
	//	for (int j = 0; j < strLen - 1; j++) {
	//		for (int k = 0, oi = 0; ; oi++) {
	//			if (oi != i)
	//				str[k++] = ori[oi];
	//			if (ori[oi] == '\0')
	//				break;
	//		}
	//		// start-end ó��
	//		for (char m = '0'; m < '5'; m++) {
	//			str[j] = m;
	//			// stró��
	//		}
	//	}
	//} 

	// 1�� 1��
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
	//		// start-end ó��
	//		for (char m = '0'; m < '5'; m++) {
	//			str[j] = m;
	//			// stró��
	//		}
	//	}
	//}
	
	// 1�� 1��
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
	//			// start-end ó��
	//			for (char m = '0'; m < '5'; m++) {
	//				str[i] = m;
	//				// start-end ó��
	//				for (char n = '0'; n < '5'; n++) {
	//					str[j] = n;
	//					// str ó��
	//				}
	//			}
	//		}
	//	}
	//}
}