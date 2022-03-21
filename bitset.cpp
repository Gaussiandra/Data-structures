#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned baseType;
#define BITS_PER_VAR (8 * sizeof(baseType))
#define getSetIndicies(elem)                \
    size_t chunkIdx = elem / BITS_PER_VAR,  \
           bitIdx   = elem % BITS_PER_VAR;

struct set {
    baseType *chunkArr;
    size_t capacity, nChunks;
};

// Создать новое пустое множество, значения элементов которого могут лежать в границах от 0 до capacity-1 включительно. Вернуть указатель на него.
struct set *set_new(size_t capacity) {
    struct set *newSet = (struct set*) calloc(1, sizeof(struct set));
    newSet->capacity = capacity;
    newSet->nChunks  = (capacity + BITS_PER_VAR - 1) / BITS_PER_VAR;
    newSet->chunkArr = (baseType*) calloc(newSet->nChunks, sizeof(baseType));
    return newSet;
}

// Добавить во множество s элемент elem. Если элемент существовал, множество не изменится. Вернуть 0, если операция корректна, т.е. elem < capacity, иначе вернуть 1.
int set_insert(struct set *s, size_t elem) {
    assert(s);

    if (elem < s->capacity) {
        getSetIndicies(elem);
        s->chunkArr[chunkIdx] |= (baseType)1 << bitIdx;

        return 0;
    }
    return 1;
}
// Удалить элемент из множества. Если элемент не существовал, множество не изменится. Вернуть 0, если операция корректна, т.е. elem < capacity, иначе вернуть 1.
int set_erase(struct set *s, size_t elem) {
    assert(s);

    if (elem < s->capacity) {
        getSetIndicies(elem);
        baseType mask = (baseType)-1 ^ ((baseType)1 << bitIdx);
        s->chunkArr[chunkIdx] &= mask;

        return 0;
    }
    return 1;
}

// Вернуть 1, если элемент присутствует в множестве и 0 в противном случае.
int set_find(struct set const *s, size_t elem) {
    assert(s);

    if (elem < s->capacity) {
        getSetIndicies(elem);
        return (int) (s->chunkArr[chunkIdx] & ((baseType)1 << bitIdx));
    }
    return 0;
}

// Удалить объект множество и вернуть NULL
struct set *set_delete(struct set *s) {
    assert(s);

    free(s->chunkArr);
    free(s);

    return NULL;
}

// Вернуть наименьший из элементов множества, не меньших start. Если таких элементов нет (например, множество пустое), то вернуть -1.
ssize_t set_findfirst(struct set const *s, size_t start)
{
    assert(s);

    if (start < s->capacity) {
        getSetIndicies(start);
        
        for (size_t chunkCnt = chunkIdx; chunkCnt < s->nChunks; ++chunkCnt) {
            for (size_t bit = 0; bit < BITS_PER_VAR; ++bit) {
                if (chunkCnt == chunkIdx && bit == 0) {
                    bit = bitIdx;
                }

                if (s->chunkArr[chunkCnt] & (1 << bit)) {
                    return (ssize_t) (chunkCnt * BITS_PER_VAR + bit);
                }
            }
        }
    }
    return -1; 
}

// Вернуть количество элементов множества.
size_t set_size(struct set const *s) {
    assert(s);

    size_t size = 0;
    for (size_t chunkCnt = 0; chunkCnt < s->nChunks; ++chunkCnt) {
        size += (size_t) __builtin_popcount(s->chunkArr[chunkCnt]);
    }

    return size;
}

// Предикат: вернуть единицу, если в множестве есть хотя бы один элемент и ноль в противном случае.
int set_empty(struct set const *s) {
    assert(s);
    
    return set_size(s) > 0;
}

// Вывести на стандартный вывод содержимое множества в формате по образцу: []\n или [1, 2, 3]\n.
void set_print(struct set const *s) {
    assert(s);

    printf("[");

    size_t nPrinted = 0, size = set_size(s);
    for (size_t chunkCnt = 0; chunkCnt < s->nChunks; ++chunkCnt) {
        for (size_t bit = 0; bit < BITS_PER_VAR; ++bit) {
            if (s->chunkArr[chunkCnt] & (1 << bit)) {
                printf("%lu", chunkCnt * BITS_PER_VAR + bit);

                if (++nPrinted < size) {
                    printf(", ");
                }
            }
        }
    }

    printf("]\n");
}
            
int main() {
    struct set *s = set_new(10);

    set_insert(s, 0);
    set_insert(s, 1);
    set_insert(s, 2);
    set_insert(s, 3);
    set_insert(s, 4);
    set_insert(s, 5);
    set_insert(s, 6);
    set_insert(s, 7);
    set_insert(s, 8);
    set_insert(s, 9);
    set_insert(s, 10);

    set_erase(s, 0);
    set_erase(s, 9);
    set_erase(s, 4);
    set_erase(s, 1);

    set_print(s);            

    assert(set_delete(s) == NULL);
}