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

inline base_t getSum(base_t arr[], size_t r) {
    base_t sum = 0;
    for (size_t i = r; i >= 1; i -= rg(i)) {
        sum += arr[i];
    }

    return sum;
}

inline base_t getSum(base_t arr[], size_t l, size_t r) {

    if (l > r) {
        return 0;
    }
    else if (l == r) {
        return arr[l];
    }
    else {
        return getSum(arr, r) - getSum(arr, l - 1);
    }
}

void changeValue(base_t arr[], size_t n, size_t idx, base_t value) {
    base_t delta = arr[idx] - value;
    for ( ; idx <= n; idx += rg(idx)) {
        arr[idx] += delta;
    }
}

base_t *buildFenwick(base_t arr[], size_t n) {
    base_t *fenwick = (base_t*) calloc(n + 1, sizeof(base_t));

    fenwick[1] = arr[1];
    for (size_t i = 2; i < n + 1; ++i) {
        fenwick[i] = getSum(fenwick, i - rg(i) + 1, i - 1) + arr[i];
    }

    return fenwick;
}

int main() {
    size_t n = 0;
    scanf("%zu", &n);
    base_t *input = (base_t*) calloc(n + 1, sizeof(base_t));

    for (size_t i = 1; i < n + 1; ++i) {
        base_t curVal = 0;
        scanf(baseSpec, &curVal);
        input[i] = curVal;
    }

    base_t *fenwick = buildFenwick(input, n);

    for (int i = 0; i < n + 1; ++i) {
        printf("%d ", fenwick[i]);
    }
    printf("\n");

    printf(baseSpec, getSum(fenwick, 1, 8));

    free(fenwick);
}