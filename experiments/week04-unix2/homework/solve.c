#include "header.h"

int pow10(int a) {
    int p = 1;
    while (a--) {
        p *= 10;
    }
    return p;
}

void calc(int prefix, int size, int *count) {
    if (prefix == 0) {
        if (size == 0)
            return;
        for (int i = 0; i < 10; i++)
            count[i] += size * pow10(size - 1);
        for (int i = 1; i < size; i++) {
            count[0] -= 9 * pow10(size - 1 - i) * i;
        }
        count[0] -= size;
    } else {
        while (prefix) {
            count[prefix % 10] += pow10(size);
            prefix /= 10;
        }
        if (size == 0)
            return;
        for (int i = 0; i < 10; i++)
            count[i] += size * pow10(size - 1);
    }
}

void solve(int n) {
    int *count = (int*) calloc(10, sizeof(int));
   
    n++;
    int sum = 0;
    while (sum < n) {
        for (int i = 0;; i++) {
            if (sum + pow10(i) <= n) continue;
            calc(sum / pow10(i - 1), i - 1, count);
            sum += pow10(i - 1);
            break;
        }
    }

    printarr(count, 10);
    free(count);
}