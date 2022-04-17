#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef int base_t;

enum heapType {
    MIN = 1,
    MAX = -1
};

struct heap_t {
    base_t *arr;
    size_t capacity, size;
    enum heapType type;
};

struct heap_t * heapCreate(size_t capacity, enum heapType type);
void heapInsert(struct heap_t  *heap, base_t value);
base_t heapExtract(struct heap_t  *heap);
void heapSetValue(struct heap_t  *heap, size_t idx, base_t value);
void heapMerge(struct heap_t  *heapDst, struct heap_t  *heapSrc);
void heapify(struct heap_t  *heap);
static void heapReallocIfNeeded(struct heap_t  *heap, size_t newCapacity);
static void heapSiftUp(struct heap_t  *heap, size_t idx);
static void heapSiftDown(struct heap_t  *heap, size_t idx);
static void heapSwap(struct heap_t  *heap, size_t a, size_t b);
void heapDelete(struct heap_t  *heap);

struct heap_t * heapCreate(size_t capacity, enum heapType type) {
    struct heap_t  *newHeap = (struct heap_t *) calloc(1, sizeof(struct heap_t ));
    newHeap->arr = (base_t*) calloc(capacity + 1, sizeof(base_t));
    newHeap->capacity = capacity + 1;
    newHeap->size = 0;
    newHeap->type = type;

    return newHeap;
} 

void heapInsert(struct heap_t  *heap, base_t value) {
    assert(heap);

    heapReallocIfNeeded(heap, heap->size + 1);

    heap->arr[++heap->size] = value;
    heapSiftUp(heap, heap->size);
}

base_t heapExtract(struct heap_t  *heap) {
    assert(heap);
    assert(heap->size);

    base_t toReturn = heap->arr[1];

    heapSwap(heap, 1, heap->size--);
    heapSiftDown(heap, 1);

    return toReturn;
}

void heapSetValue(struct heap_t  *heap, size_t idx, base_t value) {
    assert(heap);

    base_t delta = value - heap->arr[idx];

    heap->arr[idx] = value;
    if (delta > 0) {
        heapSiftDown(heap, idx);
    }
    else if (delta < 0) {
        heapSiftUp(heap, idx);
    }
}

void heapMerge(struct heap_t  *heapDst, struct heap_t  *heapSrc) {
    assert(heapDst);
    assert(heapSrc);

    heapReallocIfNeeded(heapDst, heapDst->size + heapSrc->size);

    memcpy(heapDst->arr + heapDst->size + 1,
           heapSrc->arr + 1,
           heapSrc->size * sizeof(base_t));
    heapDst->size += heapSrc->size;
    heapify(heapDst);
}

void heapify(struct heap_t  *heap) {
    assert(heap);

    for (size_t i = heap->size / 2; i > 0; --i) {
        heapSiftDown(heap, i);
    }
}

static void heapReallocIfNeeded(struct heap_t  *heap, size_t neededIdx) {
    assert(heap);

    if (heap->capacity < neededIdx + 1) {
        size_t newCapacity = max(heap->capacity * 2, neededIdx + 1);
        heap->arr = (base_t*) realloc(
            heap->arr, newCapacity * sizeof(base_t));
        assert(heap->arr);
        heap->capacity = newCapacity;
    }
}

static void heapSiftUp(struct heap_t  *heap, size_t idx) {
    assert(heap);

    int s = heap->type;
    for (size_t i = idx;
         i > 1 && s*heap->arr[i] < s*heap->arr[i / 2];
         i /= 2) {
        heapSwap(heap, i, i / 2);
    }  
}

static void heapSiftDown(struct heap_t  *heap, size_t idx) {
    assert(heap);

    while (idx * 2 <= heap->size) {
        size_t l = idx * 2,
               r = l + 1;

        int s = heap->type;
        size_t smallest = 0;
        if (r <= heap->size && s*heap->arr[r] < s*heap->arr[l]) {
            smallest = r;
        }
        else {
            smallest = l;
        }
        if (s*heap->arr[idx] <= s*heap->arr[smallest]) {
            break;
        }
        heapSwap(heap, idx, smallest);
        idx = smallest;
    }
}

static void heapSwap(struct heap_t  *heap, size_t a, size_t b) {
    assert(heap);

    base_t tmp = heap->arr[a];
    heap->arr[a] = heap->arr[b];
    heap->arr[b] = tmp;
}

void heapDelete(struct heap_t  *heap) {
    assert(heap);

    free(heap->arr);
    free(heap);
}

int main() {
    int n = 0;
    scanf("%d", &n);
    struct heap_t* heaps[6] = {};
    
    int value = 0;
    while (scanf("%d", &value) != EOF) {
        switch (value)
        {
        case 0: {
            int idx = 0, size = 0, type = 0;
            scanf("%d %d %d", &idx, &size, &type);
            heaps[idx] = heapCreate(size, type == 0 ? MIN : MAX);
            break;
        }
        case 1: {
            int idx = 0, elem = 0;
            scanf("%d %d", &idx, &elem);
            heapInsert(heaps[idx], elem);
            break;
        }
        case 2: {
            int idx = 0;
            scanf("%d", &idx);
            printf("%d\n", heaps[idx]->arr[1]);
            break;
        }
        case 3: {
            int idx = 0;
            scanf("%d", &idx);
            heapExtract(heaps[idx]);
            break;
        }
        case 4: {
            int idx = 0;
            scanf("%d", &idx);
            printf("%zu\n", heaps[idx]->size);
            break;
        }
        case 5: {
            int idx1 = 0, idx2 = 0;
            scanf("%d %d", &idx1, &idx2);
            heapMerge(heaps[idx1], heaps[idx2]);
            heaps[idx2]->size = 0;
            break;
        }
        case 6: {
            int idx = 0;
            scanf("%d", &idx);
            heapDelete(heaps[idx]);
            break;
        }
        default:
            break;
        }
    }
}