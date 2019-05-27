#include "tetris.h"
#include "decision_tree.h"
#include "ordered_list.h"
#include "queue.h"
#include <time.h>
#include <sys/time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define ABS(a) ((a) > 0 ? (a) : -(a))

// #define DEBUG
// #define DEBUG_GARBAGE_MODE

static struct sigaction act, oact;

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
char visit[HEIGHT + 8][WIDTH + 8][4];

struct timeval startTime, endTime;
double recommendTimeSum = 0; // in millisecs
long long recommendSpaceSum = 0;

ordered_list rankList;

#ifdef DEBUG
int totalLineClears = 0;
int lineClears[5];
#endif

int main() {
    int exit = 0;

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    if (!can_change_color()) {
        Color = ColorFallback;
        ShadowColor = ShadowColorFallback;
    } else {
        Color = Color256;
        ShadowColor = ShadowColor256;
    }

    start_color();
    for (int i = 0; i < 7; i++) {
        init_pair(Color[i], Color[i], COLOR_BLACK);
        init_pair(ShadowColor[i], ShadowColor[i], COLOR_BLACK);
    }

    createRankList();

    srand((unsigned int)time(NULL));

    while (!exit) {
        clear();
        switch (menu()) {
        case MENU_PLAY:
            play();
            break;
        case MENU_RANK:
            rank();
            break;
        case MENU_RECOMMEND_PLAY:
            recommendedPlay();
            break;
        case MENU_EXIT:
            exit = 1;
            break;
        default:
            break;
        }
    }

    writeRankFile();

    endwin();
    system("clear");
    return 0;
}

void InitTetris() {
    for (int j = 0; j < HEIGHT; j++)
        for (int i = 0; i < WIDTH; i++)
            field[j][i] = 0;

#ifdef DEBUG_GARBAGE_MODE
    for (int j = HEIGHT - 10; j < HEIGHT; j++) {
        int hole = rand() % WIDTH;
        for (int i = 0; i < WIDTH; i++) {
            field[j][i] = ShadowColor[j % 7];
        }
        field[j][hole] = 0;
    }
#endif

    for (int i = 0; i < BLOCK_NUM; i++) {
        nextBlock[i] = rand() % 7;
    }

    blockRotate = 0;
    blockY = -1;
    blockX = WIDTH / 2 - 2;
    score = 0;
    gameOver = 0;
    timed_out = 0;
    droppedBlocks = 0;

    DrawOutline();
    DrawField();
    DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ', 0);
    DrawNextBlock(nextBlock);
    PrintScore(score);

    decision_tree_node root = decision_tree_create();
    recommend(root);
}

void DrawOutline() {
    int i, j;
    /* 블럭이 떨어지는 공간의 태두리를 그린다.*/
    DrawBox(0, 0, HEIGHT, WIDTH);

    /* next block을 보여주는 공간의 태두리를 그린다.*/
    move(2, WIDTH + 10);
    printw("NEXT BLOCK");
    for (int i = 0; i < BLOCK_NUM - 1; i++) {
        DrawBox(3 + i * 6, WIDTH + 10, 4, 8);
    }

    /* score를 보여주는 공간의 태두리를 그린다.*/
    move(-3 + 6 * BLOCK_NUM, WIDTH + 10);
    printw("SCORE");
    DrawBox(-2 + 6 * BLOCK_NUM, WIDTH + 10, 1, 8);
}

int GetCommand() {
    int command;
    command = wgetch(stdscr);
    switch (command) {
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_LEFT:
        break;
    case KEY_RIGHT:
        break;
    case ' ': /* space key*/
        /*fall block*/
        break;
    case 'q':
    case 'Q':
        command = QUIT;
        break;
    default:
        command = NOTHING;
        break;
    }
    if (command != QUIT && flag_autoplay) {
        return KEY_DOWN;
    }
    return command;
}

int ProcessCommand(int command) {
    int ret = 1;
    int drawFlag = 0;
    switch (command) {
    case QUIT:
        ret = QUIT;
        break;
    case KEY_UP:
        if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4,
                                    blockY, blockX)))
            blockRotate = (blockRotate + 1) % 4;
        break;
    case KEY_DOWN:
        if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate,
                                    blockY + 1, blockX)))
            blockY++;
        break;
    case KEY_RIGHT:
        if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY,
                                    blockX + 1)))
            blockX++;
        break;
    case KEY_LEFT:
        if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY,
                                    blockX - 1)))
            blockX--;
        break;
    case ' ':
        while ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate,
                                       blockY + 1, blockX)))
            blockY++;
        break;
    default:
        break;
    }
    if (drawFlag)
        DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
    return ret;
}

void DrawField() {
    int i, j;
    for (j = 0; j < HEIGHT; j++) {
        move(j + 1, 1);
        for (i = 0; i < WIDTH; i++) {
            if (field[j][i]) {
                attron(A_REVERSE | COLOR_PAIR(field[j][i]));
                printw(" ");
                attroff(A_REVERSE | COLOR_PAIR(field[j][i]));
            } else {
                attron(COLOR_PAIR(0));
                printw(".");
                attroff(COLOR_PAIR(0));
            }
        }
    }
}

void PrintScore(int score) {
    move(-1 + 6 * BLOCK_NUM, WIDTH + 11);
    printw("%8d", score);
}

void DrawNextBlock(int *nextBlock) {
    for (int b = 1; b < BLOCK_NUM; b++) {
        for (int i = 0; i < 4; i++) {
            move(-2 + i + 6 * b, WIDTH + 13);
            for (int j = 0; j < 4; j++) {
                if (block[nextBlock[b]][0][i][j] == 1) {
                    attron(A_REVERSE | COLOR_PAIR(Color[nextBlock[b]]));
                    printw(" ");
                    attroff(A_REVERSE | COLOR_PAIR(Color[nextBlock[b]]));
                } else
                    printw(" ");
            }
        }
    }
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile,
               int isShadow) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
                move(i + y + 1, j + x + 1);
                if (isShadow) {
                    attron(A_REVERSE | COLOR_PAIR(ShadowColor[blockID]));
                    printw("%c", tile);
                    attroff(A_REVERSE | COLOR_PAIR(ShadowColor[blockID]));
                } else {
                    attron(A_REVERSE | COLOR_PAIR(Color[blockID]));
                    printw("%c", tile);
                    attroff(A_REVERSE | COLOR_PAIR(Color[blockID]));
                }
            }
        }
    }

    move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
    int i, j;
    move(y, x);
    addch(ACS_ULCORNER);
    for (i = 0; i < width; i++)
        addch(ACS_HLINE);
    addch(ACS_URCORNER);
    for (j = 0; j < height; j++) {
        move(y + j + 1, x);
        addch(ACS_VLINE);
        move(y + j + 1, x + width + 1);
        addch(ACS_VLINE);
    }
    move(y + j + 1, x);
    addch(ACS_LLCORNER);
    for (i = 0; i < width; i++)
        addch(ACS_HLINE);
    addch(ACS_LRCORNER);
}

void play() {
    gettimeofday(&startTime, NULL);

    struct itimerval it_val;
    it_val.it_value.tv_sec = AUTOPLAY_DROP_INTERVAL / 1000;
    it_val.it_value.tv_usec = (AUTOPLAY_DROP_INTERVAL * 1000) % 1000000;
    it_val.it_interval = it_val.it_value;

    int command;
    clear();
    act.sa_handler = BlockDown;
    sigaction(SIGALRM, &act, &oact);
    InitTetris();
    do {
        if (timed_out == 0) {
            if (flag_autoplay) {
                setitimer(ITIMER_REAL, &it_val, NULL);
            } else {
                alarm(1);
            }
            timed_out = 1;
        }

        command = GetCommand();
        if (ProcessCommand(command) == QUIT) {
            alarm(0);
            DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
            move(HEIGHT / 2, WIDTH / 2 - 4);
            printw("Good-bye!!");
            refresh();
            getch();

            return;
        }
#ifndef     DEBUG
        gettimeofday(&endTime, NULL);
        double duration = (endTime.tv_usec - startTime.tv_usec) / 1000 + (endTime.tv_sec - startTime.tv_sec) * 1000;
        
        move(3, WIDTH + 25);
        printw("time                 = %10.2f   s", duration / 1000);
        move(4, WIDTH + 25);
        printw("blocks               = %10d   b", droppedBlocks);

        move(6, WIDTH + 25);
        printw("score per time       = %10.2f / s", score / duration * 1000);
        move(7, WIDTH + 25);
        printw("score per blocks     = %10.2f / b", (double) score / droppedBlocks);

        move(9, WIDTH + 25);
        printw("recommend calc time  = %10.2f   s", recommendTimeSum / 1000);
        move(10, WIDTH + 25);
        if (recommendSpaceSum < 1000) {
            printw("recommend calc space = %10.2f   B", (double)recommendSpaceSum);
        } else if (recommendSpaceSum < 1e6) {
            printw("recommend calc space = %10.2f  KB", (double)recommendSpaceSum / 1000);
        } else if (recommendSpaceSum < 1e9) {
            printw("recommend calc space = %10.2f  MB", (double)recommendSpaceSum / 1e6);
        } else if (recommendSpaceSum < 1e12) {
            printw("recommend calc space = %10.2f  GB", (double)recommendSpaceSum / 1e9);
        } else if (recommendSpaceSum < 1e15) {
            printw("recommend calc space = %10.2f  TB", (double)recommendSpaceSum / 1e12);
        } else {
            printw("recommend calc space = %10.2f  PB", (double)recommendSpaceSum / 1e15);
        }

        move(12, WIDTH + 25);
        printw("score per calc time  = %10.2f / s", score / recommendTimeSum * 1000);
        move(13, WIDTH + 25);
        double spaceEff = (double) score / recommendSpaceSum;
        if (spaceEff >= 1) {
            printw("score per calc space = %10.2f / B", (double) score / recommendSpaceSum);
        } else if (spaceEff >= 1e-3) {
            printw("score per calc space = %10.2f /KB", (double) score / recommendSpaceSum * 1e3);
        } else if (spaceEff >= 1e-6) {
            printw("score per calc space = %10.2f /MB", (double) score / recommendSpaceSum * 1e6);
        } else if (spaceEff >= 1e-9) {
            printw("score per calc space = %10.2f /GB", (double) score / recommendSpaceSum * 1e9);
        } else if (spaceEff >= 1e-12) {
            printw("score per calc space = %10.2f /TB", (double) score / recommendSpaceSum * 1e12);
        } else {
            printw("score per calc space = %10.2f /PB", (double) score / recommendSpaceSum * 1e15);
        }
#endif
    } while (!gameOver);

    alarm(0);
    getch();
    DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
    move(HEIGHT / 2, WIDTH / 2 - 4);
    printw("GameOver!!");

    refresh();
    getch();
    if (!flag_autoplay)
        newRank(score);
}

char menu() {
    printw("1. play\n");
    printw("2. rank\n");
    printw("3. recommended play\n");
    printw("4. exit\n");
    return wgetch(stdscr);
}

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate,
                int blockY, int blockX) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][blockRotate][j][i]) {
                int x = blockX + i;
                int y = blockY + j;
                if (!(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT))
                    return false;
                if (f[y][x])
                    return false;
            }
        }
    }
    return true;
}

int GhostY(int y, int x, int blockID, int blockRotate) {
    while (CheckToMove(field, nextBlock[0], blockRotate, y + 1, x))
        y++;
    return y;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock,
                int blockRotate, int blockY, int blockX) {
    DrawField();
    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
    if (flag_autoplay && CheckToMove(field, nextBlock[0], recRot, recY, recX)) {
        blockX = recX;
        blockY = recY;
        blockRotate = recRot;
    }

    if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    } else {
        score +=
            AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);

        if (blockY == -1) {
            gameOver = true;
        } else {
            score += DeleteLine(field);

#ifdef DEBUG_GARBAGE_MODE
            if (droppedBlocks % 15 == 1) {
                for (int j = 0; j < HEIGHT - 1; j++) {
                    for (int i = 0; i < WIDTH; i++) {
                        field[j][i] = field[j + 1][i];
                    }
                }

                int randomColor = rand() % 7, holeX = rand() % WIDTH;
                for (int i = 0; i < WIDTH; i++) {
                    field[HEIGHT - 1][i] = ShadowColor[randomColor];
                }
                field[HEIGHT - 1][holeX] = 0;
            }
#endif

            for (int i = 0; i < BLOCK_NUM - 1; i++) {
                nextBlock[i] = nextBlock[i + 1];
            }
            nextBlock[BLOCK_NUM - 1] = rand() % 7;
            DrawNextBlock(nextBlock);

            blockY = -1, blockX = WIDTH / 2 - 2;
            DrawField();
            PrintScore(score);
            droppedBlocks++;

            decision_tree_node root = decision_tree_create();
            recommend(root);
        }
    }
    timed_out = 0;

#ifdef DEBUG
    move(HEIGHT + 3, 3);
    printw("Total lines:     %8d     ", totalLineClears);

    for (int i = 1; i <= 4; i++) {
        double ratio = (double) lineClears[i] / totalLineClears * 100;

        move(HEIGHT + 4 + i, 3);
        if (i == 1) {
            printw("%d line : %6.2f%% %8d     ", i, ratio, lineClears[i]);
        } else {
            printw("%d lines: %6.2f%% %8d     ", i, ratio, lineClears[i]);
        }

        for (int j = 0; j < 15; j++) {
            move(HEIGHT + 4 + i, 30 + j);
            if (ratio / 100 * 15 > j) {
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            } else {
                printw(" ");
            }
        }
    }
#endif
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate,
                    int blockY, int blockX) {
    int adjacentCount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!block[currentBlock][blockRotate][i][j])
                continue;

            int ny = blockY + i, nx = blockX + j;
            if (0 > ny || ny >= HEIGHT)
                continue;
            if (0 > nx || nx >= WIDTH)
                continue;

            f[ny][nx] = Color[currentBlock];
            if (ny == HEIGHT - 1)
                adjacentCount++;
            else if (f[ny + 1][nx] != 0)
                adjacentCount++;
        }
    }
    return adjacentCount * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
    int erased = 0;
    for (int i = 0; i < HEIGHT; i++) {
        int flag = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (!f[i][j])
                flag = 0;
        }
        if (flag) {
            erased++;
            for (int y = i; y >= 1; y--) {
                for (int x = 0; x < WIDTH; x++) {
                    f[y][x] = f[y - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) {
                f[0][x] = 0;
            }
            i--;
        }
    }

#ifdef DEBUG
    if (erased) {
        totalLineClears++;
        lineClears[erased]++;
    }
#endif

    return 100 * erased * erased;
}

void DrawShadow(int y, int x, int blockID, int blockRotate) {
    y = GhostY(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, '/', 1);
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
    DrawRecommend(recY, recX, recBlock, recRot);
    if (!flag_autoplay)
        DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ', 0);
}

void createRankList() {
    FILE *fp;

    fp = fopen("rank.txt", "r");

    if (fp == NULL) {
        fp = fopen("rank.txt", "w");
        if (fp == NULL) {
            printf("error: cannot create ranking\n");
            exit(0);
        }
        fprintf(fp, "0\n");
        rankList = newList();
    } else {
        int n;
        char name[NAMELEN];
        rankList = newList();
        fscanf(fp, "%d", &n);

        for (int i = 0; i < n; i++) {
            int score;
            fscanf(fp, "%s%d", name, &score);
            orderedList_Insert(rankList, newEntry(score, name));
        }
    }

    fclose(fp);

    flag_rank_invalidate = 0;
}

void rank() {
    int command, x, y, flag;
    char name[NAMELEN];
    ordered_list_node *query_entries = NULL;

    do {
        clear();
        printw("1. list ranks from X to Y\n");
        printw("2. list ranks by a specific name\n");
        printw("3. delete a specific rank\n");
        command = wgetch(stdscr);
    } while ('0' >= command || command >= '4');

    echo();

    switch (command) {
    case '1':
        printw("X: ");
        scanw("%d", &x);
        printw("Y: ");
        scanw("%d", &y);

        if (1 > x || x > rankList->size) {
            x = 1;
        }
        if (1 > y || y > rankList->size) {
            y = rankList->size;
        }

        printw(" rank |       name       |   score   \n");
        printw("-------------------------------------\n");

        if (x > y) {
            printw("\nsearch failure: no rank in the list\n");
        } else {
            query_entries = orderedList_Query(rankList, x, y);

            for (int i = 0; i < y - x + 1; i++) {
                printw(" %4d | %-17s| %-10d\n", i + x, query_entries[i]->name,
                       query_entries[i]->score);
            }

            if (query_entries) {
                free(query_entries);
            }
        }
        break;
    case '2':
        printw("input the name: ");
        scanw("%s", name);

        // get list of all ranks and search for matching name
        printw(" rank |       name       |   score   \n");
        printw("-------------------------------------\n");
        flag = 0;
        query_entries = orderedList_Query(rankList, 1, rankList->size);
        for (int i = 0; i < rankList->size; i++) {
            if (strcmp(name, query_entries[i]->name) == 0) {
                printw(" %4d | %-17s| %-10d\n", i + 1, query_entries[i]->name,
                       query_entries[i]->score);
                flag = 1;
            }
        }
        if (!flag) {
            printw("\nsearch failure: no name in the list\n");
        }
        if (query_entries) {
            free(query_entries);
        }
        break;
    case '3':
        printw("input the rank: ");
        scanw("%d", &x);

        if (orderedList_Delete(rankList, x) == 0) {
            printw("result: the rank deleted\n");
            flag_rank_invalidate = 1;
        } else {
            printw("search failure: the rank is not in the list\n");
        }
        break;
    }
    noecho();
    getch();
}

void writeRankFile() {
    FILE *fp;
    ordered_list_node *query_entries;

    if (!flag_rank_invalidate) {
        return;
    }

    fp = fopen("rank.txt", "wt");
    if (fp == NULL) {
        printf("error: cannot open rank file");
        exit(0);
    } else {
        query_entries = orderedList_Query(rankList, 1, rankList->size);

        fprintf(fp, "%d\n", rankList->size);
        for (int i = 0; i < rankList->size; i++) {
            fprintf(fp, "%s %d\n", query_entries[i]->name,
                    query_entries[i]->score);
        }

        if (query_entries) {
            free(query_entries);
        }
        freeList(rankList);
        fclose(fp);
    }
}

void newRank(int score) {
    char name[NAMELEN];

    clear();
    echo();
    printw("Input your name: ");
    scanw("%s", name);
    noecho();

    if (name[0] == '\0') {
        return;
    }

    ordered_list_node new_node = newEntry(score, name);
    orderedList_Insert(rankList, new_node);
    int rank = orderedList_indexOf(rankList, new_node);
    flag_rank_invalidate = 1;

    int n = rankList->size;
    if (rankList->size > 5) {
        n = 5;
    }

    printw(" rank |       name       |   score   \n");
    printw("-------------------------------------\n");
    ordered_list_node *query_entries = orderedList_Query(rankList, 1, n);
    for (int i = 0; i < n; i++) {
        if (i == rank - 1) {
            attron(A_REVERSE);
            printw(" %4d | %-17s| %-10d\n", i + 1, query_entries[i]->name,
                   query_entries[i]->score);
            attroff(A_REVERSE);
        } else {
            printw(" %4d | %-17s| %-10d\n", i + 1, query_entries[i]->name,
                   query_entries[i]->score);
        }
    }

    if (rank > n) {
        attron(A_REVERSE);
        printw(" %4d | %-17s| %-10d\n", rank, name, score);
        attroff(A_REVERSE);
    }

    if (query_entries) {
        free(query_entries);
    }
    getch();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
    DrawBlock(y, x, blockID, blockRotate, 'R', 1);
}

score_pair boardScore(char f[HEIGHT][WIDTH], int nextBlock, int rot, int blockY,
                       int blockX) {
    int startY[WIDTH];
    memset(startY, 0, sizeof(startY));

    int dropHeight = HEIGHT - blockY - 1;
    int adjacent = AddBlockToField(f, nextBlock, rot, blockY, blockX);

    // Clear line
    int lineClears = 0;
    for (int i = 0; i < HEIGHT; i++) {
        int flag = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (!f[i][j])
                flag = 0;
        }
        if (flag) {
            lineClears++;
            for (int y = i; y >= 1; y--) {
                for (int x = 0; x < WIDTH; x++) {
                    f[y][x] = f[y - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) {
                f[0][x] = 0;
            }
            i--;
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        int flag = 0;
        for (int y = 0; y < HEIGHT; ++y) {
            if (f[y][x]) {
                startY[x] = y;
                flag = 1;
                break;
            }
        }
        if (!flag) startY[x] = HEIGHT;
    }

    // Calculate column heights
    int maxStartY = startY[0], minStartY = startY[0];
    int totalHeights = startY[0], fuzziness = 0;
    for (int x = 1; x < WIDTH; x++) {
        minStartY = MIN(minStartY, startY[x]);
        maxStartY = MAX(maxStartY, startY[x]);
        fuzziness += ABS(startY[x] - startY[x - 1]);
        totalHeights += HEIGHT - startY[x];
    }
    int maxHeightDifference = maxStartY - minStartY;
    int pileHeight = HEIGHT - minStartY;

    // Calculate board heuristics
    int holes = 0, weighedHoles = 0, holeDepthSum = 0;
    int minHoleDepth = HEIGHT, maxHoleDepth = 0;
    int weightedValleys = 0, deepValleys = 0;
    int filledCells = 0, weightedCells = 0;

    for (int x = 0; x < WIDTH; x++) {
        // Holes
        for (int y = HEIGHT - 1; y >= startY[x] + 1; y--) {
            if (!f[y][x] && f[y - 1][x]) {
                holes++;
                weighedHoles += y + 1;

                int holeDepth = y - startY[x];
                holeDepthSum += holeDepth;
                minHoleDepth = MIN(minHoleDepth, holeDepth);
                maxHoleDepth = MAX(maxHoleDepth, holeDepth);
            }
        }

        // Valleys
        int valleyCells = 0;
        int adjacentY = MIN((x == 0 ? HEIGHT : startY[x - 1]), (x + 1 == WIDTH ? HEIGHT : startY[x + 1]));
        for (int y = startY[x] - 1; y >= adjacentY; y--) {
            if ((x == 0 || f[y][x - 1]) && (x + 1 == WIDTH || f[y][x + 1])) {
                valleyCells++;
            } 
        }
        weightedValleys += valleyCells;
        if (valleyCells > 2) {
            deepValleys++;
        }

        // Filled cells
        for (int y = HEIGHT - 1; y >= startY[x]; y--) {
            if (f[y][x]) {
                filledCells++;
                weightedCells += HEIGHT - y;
            }
        }
    }

    int weightedHighCells = 0;
    for (int y = 10; y >= minStartY; y--) {
        for (int x = 0; x < WIDTH; x++) {
            if (f[y][x]) {
                weightedHighCells += 11 - y;
            }
        }
    }
    
    int rowChunks = 0;
    for(int y = HEIGHT - 1; y >= minStartY; y--) {
        if (!f[y][0] || !f[y][WIDTH - 1]) {
            rowChunks++;
        }
        for (int x = 1; x < WIDTH; x++) {
            if (f[y][x] != f[y][x - 1]) {
                rowChunks++;
            }
        }
    }
    
    int tetris = (lineClears == 4) ? 1 : 0;

    long double propagatedScore
        = feature_weights[0]  * lineClears
        + feature_weights[1]  * dropHeight;

    long double boardScore
        = tetris_weight       * tetris
        + feature_weights[2]  * totalHeights
        + feature_weights[3]  * fuzziness
        + feature_weights[4]  * maxHeightDifference
        + feature_weights[5]  * pileHeight
        + feature_weights[6]  * holes
        + feature_weights[7]  * weighedHoles
        + feature_weights[8]  * holeDepthSum
        + feature_weights[9]  * minHoleDepth
        + feature_weights[10] * maxHoleDepth
        + feature_weights[11] * weightedValleys
        + feature_weights[12] * deepValleys
        + feature_weights[13] * filledCells
        + feature_weights[14] * weightedCells
        + feature_weights[15] * weightedHighCells
        + feature_weights[16] * rowChunks;
        
    return (score_pair) {boardScore / 10000, propagatedScore / 10000};
}

decision_tree_node recommend_bfs(int level, decision_tree_node *parents,
                                 int parent_count) {
    if (level == BLOCK_NUM) {
        long double max_score = 987654321;
        decision_tree_node max_node = NULL;

        for (int i = 0; i < parent_count; i++) {
            decision_tree_node node = parents[i];
            if (max_score > node->score.boardScore + node->score.propagatedScore) {
                max_node = node;
                max_score = node->score.boardScore + node->score.propagatedScore;
            }
        }

        if (max_node == NULL)
            return NULL;
        while (max_node->parent != NULL && max_node->parent->parent != NULL) {
            max_node = max_node->parent;
        }
        return max_node;
    }

    decision_tree_node *childs = (decision_tree_node *)malloc(0);
    int child_count = 0;

    for (int i = 0; i < parent_count; i++) {
        decision_tree_node node = parents[i];

        int rotate_limit = 4;
        if (nextBlock[level] == O)
            rotate_limit = 1;
        if (nextBlock[level] == I || nextBlock[level] == S ||
            nextBlock[level] == N)
            rotate_limit = 2;

        memset(visit, 0, sizeof(visit));
        queue q = queue_create();
        queue avilable_spots = queue_create();

        for (int rot = 0; rot < rotate_limit; rot++) {
            for (int x = -4; x < WIDTH + 4; x++) {
                if (!(CheckToMove(node->field, nextBlock[level], rot, 1, x)))
                    continue;
                queue_emplace(q, (triple){5, x + 4, rot});
            }
        }

        while (q->size) {
            triple t = queue_front(q);
            queue_pop(q);

            int y = t.first - 4;
            int x = t.second - 4;
            int r = t.third;

            // move position
            for (int d = 0; d < 4; d++) {
                int ny = y + dy[d];
                int nx = x + dx[d];
                int nr = r;
                if (-4 > ny || ny >= HEIGHT + 4)
                    continue;
                if (-4 > nx || nx >= WIDTH + 4)
                    continue;
                if (visit[ny + 4][nx + 4][nr])
                    continue;
                visit[ny + 4][nx + 4][nr] = 1;
                if (!CheckToMove(node->field, nextBlock[level], nr, ny, nx)) {
                    if (d == 3) {
                        queue_emplace(avilable_spots,
                                      (triple){y + 4, x + 4, r});
                    }
                    continue;
                }
                queue_emplace(q, (triple){ny + 4, nx + 4, nr});
            }

            // rotate
            int ny = y;
            int nx = x;
            int nr = (r + 1) % rotate_limit;
            if (-4 > ny || ny >= HEIGHT + 4)
                continue;
            if (-4 > nx || nx >= WIDTH + 4)
                continue;
            if (visit[ny + 4][nx + 4][nr])
                continue;
            visit[ny + 4][nx + 4][nr] = 1;
            if (!CheckToMove(node->field, nextBlock[level], nr, ny, nx)) continue;
            if (!CheckToMove(node->field, nextBlock[level], nr, ny + 1, nx)) {
                queue_emplace(avilable_spots, (triple){y + 4, x + 4, r});
                continue;
            }
            queue_emplace(q, (triple){ny + 4, nx + 4, nr});
        }

        queue_destroy(q);

        while (avilable_spots->size) {
            decision_tree_node newChild = decision_tree_create();
            memcpy(newChild->field, node->field, sizeof(newChild->field));

            triple t = queue_front(avilable_spots);
            queue_pop(avilable_spots);

            int y = t.first - 4;
            int x = t.second - 4;
            int rot = t.third;

            score_pair board_score = boardScore(newChild->field, nextBlock[level], rot, y, x);

            newChild->parent = node;
            newChild->score = (score_pair) {board_score.boardScore, node->score.propagatedScore + board_score.propagatedScore};
            newChild->curBlockID = nextBlock[level];
            newChild->recBlockX = x;
            newChild->recBlockY = y;
            newChild->recBlockRotate = rot;
            newChild->children_size = 0;

            decision_tree_add_child(node, newChild);

            child_count++;
            childs = (decision_tree_node *)realloc(
                childs, sizeof(decision_tree_node) * child_count);
            childs[child_count - 1] = newChild;
        }

        queue_destroy(avilable_spots);
    }

    decision_tree_node_quicksort(childs, 0, child_count - 1);

    int pruned_size =
        (child_count < TREE_PRUNE_LIMIT ? child_count : TREE_PRUNE_LIMIT);
    decision_tree_node *pruned = (decision_tree_node *)malloc(
        sizeof(decision_tree_node) * (pruned_size));
    for (int i = 0; i < pruned_size; i++) {
        pruned[i] = childs[i];
    }

    return recommend_bfs(level + 1, pruned, pruned_size);
}

int recommend(decision_tree_node root) {
    struct timeval recommendStartTime, recommendEndTime;
    gettimeofday(&recommendStartTime, NULL);

    root->parent = NULL;
    root->score = (score_pair) {0, 0};
    root->children_size = 0;
    memcpy(root->field, field, sizeof(root->field));

    decision_tree_node bestNode = recommend_bfs(0, &root, 1);
    recommendSpaceSum += decision_tree_size(root);
    
    if (bestNode == NULL) {
        decision_tree_destroy(root);
        return 0;
    }

#ifdef DEBUG
    move(1 + 6 * BLOCK_NUM, WIDTH + 11);
    printw("sc  = %8.1llf", bestNode->score);
    move(2 + 6 * BLOCK_NUM, WIDTH + 11);
    printw("eff = %8.1llf", (long double)score / droppedBlocks);
    move(3 + 6 * BLOCK_NUM, WIDTH + 11);
    printw("blk = %8d", droppedBlocks);
#endif

    recX = bestNode->recBlockX;
    recY = bestNode->recBlockY;
    recBlock = bestNode->curBlockID;
    recRot = bestNode->recBlockRotate;

    decision_tree_destroy(root);
    gettimeofday(&recommendEndTime, NULL);
    recommendTimeSum += 
        (recommendEndTime.tv_usec - recommendStartTime.tv_usec) / 1000 + (recommendEndTime.tv_sec - recommendStartTime.tv_sec) * 1000;
    return 1;
}

void recommendedPlay() {
    flag_autoplay = 1;
    play();
    flag_autoplay = 0;
}
