#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row, column, value;
} Term;

#define MAX_TERMS 1024

void readMatrix(FILE *fp, Term a[]) {
    int r, c;
    fscanf(fp, "%d%d", &r, &c);

    a = (Term*) malloc(sizeof(Term));
    a[0] = {r, c};

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int x;
            fscanf(fp, "%d", &x);
            if (x == 0) continue;

            (a[0].value)++;
            a = (Term*) realloc(a, (a[0].value + 1) * sizeof(Term));
            a[a[0].value] = {i, j, x};
        }
    }
}

void printMatrix(Term a[]) {

}

void matrixAdd(Term a[], Term b[], Term c[]) {

}

int main() {
    Term a[MAX_TERMS], b[MAX_TERMS], c[MAX_TERMS];

    FILE *fp = fopen("A.txt", "r");
    readMatrix(fp, a);
    fclose(fp);
    fp = fopen("B.txt", "r");
    readMatrix(fp, b);
    fclose(fp);

    matrixAdd(a, b, c);
    printMatrix(c);

    return 0;
}