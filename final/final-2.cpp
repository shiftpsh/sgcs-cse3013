#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

/*
	Suhyun Park (20181634)
	CSE Undergraduate, Sogang U

	Question 2.
*/

struct operation {
	char c;
	int x;
};

int ord[128];
int stack[128], sidx;
operation op[256];
int opidx;

int main() {
	int tc;
	scanf("%d", &tc);

	for (int i = 1; i <= tc; i++) {
		printf("T%d\n", i);

		int n;
		scanf("%d", &n);
		sidx = 0;
		opidx = 0;

		for (int i = 0; i < n; i++) {
			scanf("%d", &ord[i]);
		}

		int oi = 0;
		for (int i = 1; i <= n; i++) {
			op[opidx].c = 'P';
			op[opidx].x = i;
			opidx++;

			stack[sidx++] = i;
			while (sidx && ord[oi] == stack[sidx - 1]) {
				op[opidx].c = 'R';
				op[opidx].x = stack[sidx - 1];
				opidx++;
				sidx--;
				oi++;
			}
		}

		if (oi < n) {
			printf("impossible\n");
		}
		else {
			for (int i = 0; i < opidx; i++) {
				printf("%c%d\n", op[i].c, op[i].x);
			}
		}
	}

	return 0;
}