#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAXH 100
#define MAXW 100

#define MIN(a, b) ((a)>(b)?(b):(a))
#define MAX(a, b) ((a)>(b)?(a):(b))

enum {
    N, E, W, S
};

char visit_flag[MAXH][MAXW];
char horizontal_route[MAXH][MAXW - 1];
char vertical_route[MAXH - 1][MAXW];

int adjacent(int possible_dirs[], int x, int y, int h, int w) {
    int possible_dir_count = 0;

    if (y > 0) possible_dirs[possible_dir_count++] = N;
    if (x < w - 1) possible_dirs[possible_dir_count++] = E;
    if (x > 0) possible_dirs[possible_dir_count++] = W;
    if (y < h - 1) possible_dirs[possible_dir_count++] = S;

    return possible_dir_count;
}

int adjacent_not_visited(int possible_dirs[], int x, int y, int h, int w) {
    int possible_dir_count = 0;

    if (y > 0 && !visit_flag[y - 1][x]) possible_dirs[possible_dir_count++] = N;
    if (x < w - 1 && !visit_flag[y][x + 1]) possible_dirs[possible_dir_count++] = E;
    if (x > 0 && !visit_flag[y][x - 1]) possible_dirs[possible_dir_count++] = W;
    if (y < h - 1 && !visit_flag[y + 1][x]) possible_dirs[possible_dir_count++] = S;

    return possible_dir_count;
}

void removeEdges(int delete, int h, int w) {
    delete = MIN(delete, MIN(h, w) / 2);
    int possible_dirs[4];

    while (delete) {
        int x = rand() % h;
        int y = rand() % w;

        int possible_dir_count = adjacent(possible_dirs, x, y, h, w);
        if (!possible_dir_count) continue;
        int random_dir = rand() % possible_dir_count;

        switch (possible_dirs[random_dir]) {
            case N:
                if (!vertical_route[y - 1][x]) {
                    vertical_route[y - 1][x] = 1;
                    delete--;
                }
                break;
            case E:
                if (!horizontal_route[y][x]) {
                    horizontal_route[y][x] = 1;
                    delete--;
                }
                break;
            case S:
                if (!vertical_route[y][x]) {
                    vertical_route[y][x] = 1;
                    delete--;
                }
                break;
            case W:
                if (!horizontal_route[y][x - 1]) {
                    horizontal_route[y][x - 1] = 1;
                    delete--;
                }
                break;
        }
    }
}

void dfs(int x, int y, int h, int w) {
    int possible_dirs[4];
    int possible_dir_count = adjacent_not_visited(possible_dirs, x, y, h, w);

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

        possible_dir_count = adjacent_not_visited(possible_dirs, x, y, h, w);
    }
}

void printMaze(int h, int w, FILE *fp) {
    for (int x = 0; x < w; x++) {
        putc('+', fp);
        putc('-', fp);
    }
    putc('+', fp);
    putc('\n', fp);
    for (int y = 0; y < h; y++) {
        putc('|', fp);
        for (int x = 0; x < w; x++) {
            putc(' ', fp);
            putc(x < w - 1 && horizontal_route[y][x] ? ' ' : '|', fp);
        }
        putc('\n', fp);
        putc('+', fp);
        for (int x = 0; x < w; x++) {
            putc(y < h - 1 && vertical_route[y][x] ? ' ' : '-', fp);
            putc('+', fp);
        }
        putc('\n', fp);
    }
}

int main() {
    srand(time(NULL));

    int h, w;
    scanf("%d%d", &h, &w);

    dfs(0, 0, h, w);
    removeEdges(10, h, w);

    FILE *fp = fopen("maze.maz", "w");
    printMaze(h, w, fp);
    fclose(fp);

    return 0;
}