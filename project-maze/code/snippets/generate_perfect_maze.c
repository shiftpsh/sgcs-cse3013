#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAXH 100
#define MAXW 100

enum {
    N, E, W, S
};

char visit_flag[MAXH][MAXW];
char horizontal_route[MAXH][MAXW - 1];
char vertical_route[MAXH - 1][MAXW];

int adjacent(int possible_dirs[], int x, int y, int h, int w) {
    int possible_dir_count = 0;

    if (y > 0 && visit_flag[y - 1][x] == 0) possible_dirs[possible_dir_count++] = N;
    if (x < w - 1 && visit_flag[y][x + 1] == 0) possible_dirs[possible_dir_count++] = E;
    if (x > 0 && visit_flag[y][x - 1] == 0) possible_dirs[possible_dir_count++] = W;
    if (y < h - 1 && visit_flag[y + 1][x] == 0) possible_dirs[possible_dir_count++] = S;

    return possible_dir_count;
}

void dfs(int x, int y, int h, int w) {
    int possible_dirs[4];
    int possible_dir_count = adjacent(possible_dirs, x, y, h, w);

    visit_flag[y][x] = 1;

    while (possible_dir_count) {
        int random_dir = rand() % possible_dir_count;

        switch (possible_dirs[random_dir]) {
            case N:
                vertical_route[y - 1][x] = 1;
                dfs(x, y - 1, h, w);
                break;
            case E:
                horizontal_route[y][x] = 1;
                dfs(x + 1, y, h, w);
                break;
            case S:
                vertical_route[y][x] = 1;
                dfs(x, y + 1, h, w);
                break;
            case W:
                horizontal_route[y][x - 1] = 1;
                dfs(x - 1, y, h, w);
                break;
        }

        possible_dir_count = adjacent(possible_dirs, x, y, h, w);
    }
}

void printMaze(int h, int w) {
    for (int x = 0; x < w; x++) {
        putchar('+');
        putchar('-');
    }
    putchar('+');
    putchar('\n');
    for (int y = 0; y < h; y++) {
        putchar('|');
        for (int x = 0; x < w; x++) {
            putchar(' ');
            putchar(x < w - 1 && horizontal_route[y][x] ? ' ' : '|');
        }
        putchar('\n');
        putchar('+');
        for (int x = 0; x < w; x++) {
            putchar(y < h - 1 && vertical_route[y][x] ? ' ' : '-');
            putchar('+');
        }
        putchar('\n');
    }
}

int main() {
    srand(time(NULL));

    int h, w;
    scanf("%d%d", &h, &w);

    dfs(0, 0, h, w);
    printMaze(h, w);

    return 0;
}