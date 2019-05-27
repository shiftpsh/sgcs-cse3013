#include "header.h"

int readint() {
	int x;
	scanf("%d", &x);
	return x;
}

void printarr(int *arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}