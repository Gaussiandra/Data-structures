#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#define max(a, b) ((a) > (b)) ? (a) : (b)
#define min(a, b) ((a) < (b)) ? (a) : (b)

#define baseSpec "%lld"
typedef long long base_t;

inline size_t rg(size_t k) {
    return k & -k;
}

inline base_t getPrefixSum(base_t arr[], size_t m,
                     size_t x2, size_t y2) {
    base_t sum = 0;
    for (size_t i = y2; i >= 1; i -= rg(i)) {
        for (size_t j = x2; j >= 1; j -= rg(j)) {
            sum += arr[i * m + j];
        }
    }

    return sum;
}

inline base_t getSum(base_t arr[], size_t m,
                     size_t x1, size_t y1,
                     size_t x2, size_t y2) {
    return getPrefixSum(arr, m, x2, y2)     - getPrefixSum(arr, m, x1 - 1, y2) -
           getPrefixSum(arr, m, x2, y1 - 1) + getPrefixSum(arr, m, x1 - 1, y1 - 1);
}

void setValue(base_t arr[], size_t n, size_t m,
                 size_t x, size_t y, base_t value) {
    base_t delta = value - getSum(arr, m, x, y, x, y);
    for (size_t i = y; i <= n; i += rg(i)) {
        for (size_t j = x; j <= m; j += rg(j)) {
            arr[i * m + j] += delta;
        }
    }
}

int main() {
    size_t n = 1002, m = 1002;
    base_t *fenwick = (base_t*) calloc(n * m, sizeof(base_t));

    char op[11] = {0};
    while (scanf("%10s", op) != EOF) {
        switch (op[0]) {
        case 'A': {
            size_t x = 0, y = 0;
            base_t v = 0;
            scanf(baseSpec" %s %zu %zu", &v, op, &x, &y);
            setValue(fenwick, n, m, x + 1, y + 1, getSum(fenwick, m, x + 1, y + 1, x + 1, y + 1) + v);
            break;
        }
        case 'R': {
            size_t x = 0, y = 0;
            scanf("%*c %zu %zu", &x, &y);
            setValue(fenwick, n, m, x + 1, y + 1, 0);
            break;
        }
        case 'C': {
            size_t x = 0, y = 0;
            base_t v = 0;
            scanf("%s %zu %zu %s "baseSpec, op, &x, &y, op, &v);
            setValue(fenwick, n, m, x + 1, y + 1, v);
            break;
        }
        case 'S': {
            size_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
            scanf("%s %zu %zu %s %zu %zu", op, &x1, &y1, op, &x2, &y2);

            printf(baseSpec"\n", getSum(fenwick, m, x1 + 1, y1 + 1, x2 + 1, y2 + 1));
            break;
        }
        default: {
            break;
        }
    }
    }
    free(fenwick);
}