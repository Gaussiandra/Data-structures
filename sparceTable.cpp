#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#define max(a, b) ((a) > (b)) ? (a) : (b)
#define min(a, b) ((a) < (b)) ? (a) : (b)

#define baseSpec "%d"
typedef int base_t;

void precalcLogs(size_t arr[], size_t n) {
    size_t curValue = 1, curLog = 0;
    for (size_t i = 1; i < n; ++i) {
        arr[i] = curLog;
        if (curValue == i) {
            curValue *= 2;
            ++curLog;
        }
    }
}

void buildSparceTable(base_t *arr, size_t n, size_t k) {
    for (int i = 1; i < k; ++i) {
        int power2 = 1 << (i-1);
        for (int j = 0; j < n - power2; ++j) {
            arr[i * n + j] = max(arr[(i-1) * n + j], arr[(i - 1) * n + j + power2]);
        }
    }
}

inline base_t getRMQ(base_t *arr, size_t *logs, size_t n, size_t k, size_t l, size_t r) {
    if (l == r) {
        return arr[l];
    }
    size_t lg = logs[r - l + 1] - 1;
    size_t rightSegmentIdx = r - (1 << lg) + 1;
    return max(arr[lg*n + l], arr[lg*n + rightSegmentIdx]);
}

int main() {
    size_t n = 0, m = 0;
    scanf("%zu %zu", &n, &m);

    size_t *logs = (size_t*) calloc(n + 1, sizeof(size_t));
    precalcLogs(logs, n + 1);

    size_t k = logs[n] + 1;
    base_t *sparceTable = (base_t*) calloc(n * k, sizeof(base_t));
    for (size_t i = 0; i < n; ++i) {
        base_t curVal = 0;
        scanf(baseSpec, &curVal);
        sparceTable[i] = curVal;
    }
    buildSparceTable(sparceTable, n, k);

    for (int i = 0; i < m; ++i) {
        size_t l = 0, r = 0;
        scanf("%zu %zu", &l, &r);

        printf(baseSpec"\n", getRMQ(sparceTable, logs, n, k, l, r));
    }

    free(logs);
    free(sparceTable);
}