#include <windows.h>
#include <stdio.h>

void test() {
	char ori[50];
	strcpy_s(ori, "ab");
	int strLen = strlen(ori);

	char str[50];

	// ���� ��
	//for (int i = 0; i < strLen + 1; i++)
	//	str[i] = ori[i];
	//// stró��

	// 1��
	//for (int i = 0; i < strLen; i++) {
	//	for (int j = 0, oi = 0; ; oi++) {
	//		if (oi != i) {
	//			str[j++] = ori[oi];
	//		}
	//		if (ori[oi] == '\0')
	//			break;
	//	}
	//	// str ó��
	//}

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