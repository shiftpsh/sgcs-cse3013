#include "tetris.h"

static struct sigaction act, oact;

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

    srand((unsigned int)time(NULL));

    while (!exit) {
        clear();
        switch (menu()) {
        case MENU_PLAY:
            play();
            break;
        case MENU_EXIT:
            exit = 1;
            break;
        default:
            break;
        }
    }

    endwin();
    system("clear");
    return 0;
}

void InitTetris() {
    int i, j;

    for (j = 0; j < HEIGHT; j++)
        for (i = 0; i < WIDTH; i++)
            field[j][i] = 0;

    for (int i = 0; i < BLOCK_NUM; i++) {
        nextBlock[i] = rand() % 7;
    }
   
    blockRotate = 0;
    blockY = -1;
    blockX = WIDTH / 2 - 2;
    score = 0;
    gameOver = 0;
    timed_out = 0;

    DrawOutline();
    DrawField();
    DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ', 0);
    DrawNextBlock(nextBlock);
    PrintScore(score);
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

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile, int isShadow) {
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
    int command;
    clear();
    act.sa_handler = BlockDown;
    sigaction(SIGALRM, &act, &oact);
    InitTetris();
    do {
        if (timed_out == 0) {
            alarm(1);
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
    } while (!gameOver);

    alarm(0);
    getch();
    DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
    move(HEIGHT / 2, WIDTH / 2 - 4);
    printw("GameOver!!");
    refresh();
    getch();
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
    while (CheckToMove(field, nextBlock[0], blockRotate, y + 1, x)) y++;
    return y;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock,
                int blockRotate, int blockY, int blockX) {
    DrawField();
    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
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

            for (int i = 0; i < BLOCK_NUM - 1; i++) {
                nextBlock[i] = nextBlock[i + 1];
            }
            nextBlock[BLOCK_NUM - 1] = rand() % 7;
            DrawNextBlock(nextBlock);

            blockY = -1, blockX = WIDTH / 2 - 2;
            DrawField();
            PrintScore(score);
        }
    }
    timed_out = 0;
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

    return 100 * erased * erased;
}

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	y = GhostY(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, '/', 1);
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ', 0);
}

void createRankList() {
    // user code
}

void rank() {
    // user code
}

void writeRankFile() {
    // user code
}

void newRank(int score) {
    // user code
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
    // user code
}

int recommend(RecNode *root) {
    int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는
                 // 최대 점수

    // user code

    return max;
}

void recommendedPlay() {
    // user code
}
