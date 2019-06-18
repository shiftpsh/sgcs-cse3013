#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

/*
	Suhyun Park (20181634)
	CSE Undergraduate, Sogang U

	Question 1.
*/

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

inline int dir(const char& c) {
	if (c == 'N') return 0;
	if (c == 'S') return 1;
	if (c == 'W') return 2;
	if (c == 'E') return 3;
	return 0;
}

bool visit[43][43];

int main() {
	int n;
	scanf("%d", &n);

	while (n--) {
		char move[82];
		scanf("%s", move);

		for (int i = 0; i <= 42; i++) {
			for (int j = 0; j <= 42; j++) {
				visit[i][j] = false;
			}
		}

		int x = 21, y = 21;

		visit[x][y] = true;
		for (const char& c : move) {
			if (c == '.') break;
			int d = dir(c);
			x += dx[d];
			y += dy[d];
			visit[x][y] = true;
		}

		int cnt = 0;
		for (int i = 0; i <= 42; i++) {
			for (int j = 0; j <= 42; j++) {
				cnt += visit[i][j];
			}
		}

		printf("%d\n", cnt);
	}

	return 0;
}