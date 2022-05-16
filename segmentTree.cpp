#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#define baseSpec "%d"
typedef int base_t;
const base_t BASE_MIN = INT_MIN;

size_t calcSTSize(size_t n) {
    int lg = (int) log2((double) n - 1);
    return 1u << (lg + 1);
}

void changeXor(base_t arr[], int idx, base_t value, int n) {
    idx += n;
    arr[idx] = value;

    for (idx /= 2; idx > 0; idx /= 2) {
        arr[idx] = arr[2 * idx] ^ arr[2 * idx + 1];
    }
}

base_t aggregateXor(base_t arr[], int a, int b, int n) {
    a += n;
    b += n;
    base_t result = 0;

    while (a <= b) {
        if (a % 2 == 1) result ^= arr[a++];
        if (b % 2 == 0) result ^= arr[b--];

        a /= 2;
        b /= 2;
    }
    
    return result;
}

void buildXor(base_t arr[], size_t size) {
    for (size_t i = size - 1; i > 0; --i) {
        arr[i] = arr[2 * i] ^ arr[2 * i + 1];
    }
}

int main() {
    size_t n = 0, m = 0;
    scanf("%zu %zu", &n, &m);

    size_t szST = calcSTSize(n);
    base_t *xorST = (base_t*) calloc(2 * szST + 1, sizeof(base_t));

    for (size_t i = 0; i < n; ++i) {
        base_t curVal = 0;
        scanf(baseSpec, &curVal);

        xorST[szST + i + 1] = curVal;
    }

    buildXor(xorST, szST);

    for (size_t i = 0; i < m; ++i) {
        int cmd = 0, arg1 = 0, arg2 = 0;
        scanf("%d %d %d", &cmd, &arg1, &arg2);

        if      (cmd == 1) printf(baseSpec"\n", aggregateXor(xorST, arg1 + 1, arg2 + 1, szST));
        else if (cmd == 2) {
            changeXor(xorST, arg1 + 1, arg2, szST);
        }
    }

    free(xorST);
}