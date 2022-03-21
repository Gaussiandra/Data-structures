#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define RETURN_ON_ERROR(func) { int error = (func); if (error) return error; }

struct vector {
    size_t size, capacity, elemSize;
    void *array;
};

static inline void* get_ptr_by_vector_index(const struct vector *vector, size_t index);
struct vector *vector_new(size_t nElems, size_t elemSize);
int vector_push(struct vector *vector, void const *element);
int vector_pop (struct vector *vector, void *element);
int vector_empty(struct vector const *vector);
struct vector *vector_delete(struct vector *vector);
void vector_print(struct vector const *vector, void (*print_func)(void const *data));
int vector_set(struct vector *vector, size_t index, void const *element);
int vector_get(struct vector const *vector, size_t index, void *element);
static int change_vector_capacity(struct vector *vector, size_t newSize);
int vector_resize(struct vector *vector, size_t new_size);
size_t vector_size(struct vector const *vector);

static inline void* get_ptr_by_vector_index(const struct vector *vector, size_t index) {
    assert(vector);
 
    return (char*) vector->array + (vector->elemSize * index);
}

struct vector *vector_new(size_t nElems, size_t elemSize) {
    assert(elemSize);

    struct vector *newVector = (struct vector*) calloc(1, sizeof(struct vector));

    newVector->size = newVector->capacity = nElems;
    newVector->elemSize = elemSize;
    newVector->array = calloc(newVector->capacity, newVector->elemSize);

    return newVector;
}

inline int vector_push(struct vector *vector, const void *element) {
    assert(vector);
    assert(element);

    if (vector->size == vector->capacity) {
        RETURN_ON_ERROR(change_vector_capacity(vector, vector->size * 2 + 1));
    }

    ++vector->size;
    RETURN_ON_ERROR(vector_set(vector, vector->size - 1, element));

    return 0;
}

inline int vector_pop(struct vector *vector, void *element) {
    assert(vector);
    assert(element);

    RETURN_ON_ERROR(vector_get(vector, vector->size - 1, element));
    --vector->size;

    return 0;
}

int vector_empty(struct vector const *vector) {
    assert(vector);

    return vector->size == 0;
}

struct vector *vector_delete(struct vector *vector) {
    assert(vector);

    free(vector->array);
    free(vector);

    return NULL;
}

void vector_print(struct vector const *vector, void (*print_func)(void const *vector)) {
    assert(vector);
    assert(print_func);

    printf("[");
    for (size_t idx = 0; vector->size && (idx < vector->size - 1); ++idx) {
        print_func(get_ptr_by_vector_index(vector, idx));
        printf(", ");
    }
    if (vector->size > 0) {
        print_func(get_ptr_by_vector_index(vector, vector->size - 1));
    }
    printf("]\n");
}

inline int vector_set(struct vector *vector, size_t index, void const *element) {
    assert(vector);
    assert(element);

    if (index >= vector->size) {
        return 1;
    }

    memcpy(get_ptr_by_vector_index(vector, index), element, vector->elemSize);
    return 0;
}

inline int vector_get(struct vector const *vector, size_t index, void *element) {
    assert(vector);
    assert(element);

    if (index >= vector->size) {
        return 1;
    }

    memcpy(element, get_ptr_by_vector_index(vector, index), vector->elemSize);
    return 0;
}

static int change_vector_capacity(struct vector *vector, size_t newSize) {
    assert(vector);

    void *newPtr = realloc(vector->array, newSize * vector->elemSize); 
    if (!newPtr) {
        return 1;
    }
    
    vector->array = newPtr;
    vector->capacity = newSize;
    return 0;
}

int vector_resize(struct vector *vector, size_t newSize) {
    assert(vector);

    if (newSize > vector->capacity) {
        change_vector_capacity(vector, newSize);
    }
    vector->size = newSize;

    return 0;
}

size_t vector_size(struct vector const *vector) {
    assert(vector);

    return vector->size;
}

//////////////////////////////////////////////////////////////
struct queue {
    struct vector *vector; 
    size_t start, end, size;
};

struct queue *queue_new(size_t elem_size);
inline int queue_push(struct queue *q, const void *elem);
inline int queue_pop(struct queue *q, void *elem);
int queue_empty(struct queue const *q);
struct queue *queue_delete(struct queue *q);
void queue_print(struct queue const *q, void (*pf)(void const *data));

struct queue *queue_new(size_t elemSize) {
    struct queue *queue = (struct queue*) calloc(1, sizeof(struct queue));
    queue->vector = vector_new(0, elemSize);
    queue->start = queue->end = queue->size = 0;
    return queue;
}

inline int queue_push(struct queue *q, const void *element) {
    assert(q);
    assert(element);

    if (q->vector->capacity == 0 || q->start == (q->end + 1) % q->vector->capacity) {
        size_t nRightElements = q->vector->capacity - q->start; //? либо выводит мусорные значения при выводе
        RETURN_ON_ERROR(vector_resize(q->vector, q->vector->capacity * 2 + 1));

        if (q->start > q->end) {
            memcpy(get_ptr_by_vector_index(q->vector, q->vector->capacity - nRightElements), 
                   get_ptr_by_vector_index(q->vector, q->start), 
                   nRightElements * q->vector->elemSize);
            q->start = q->vector->capacity - nRightElements;
        }
    }

    if (q->size > 0) {
        RETURN_ON_ERROR(vector_set(q->vector, 
                                   (q->end + 1) % q->vector->capacity, 
                                   element));
        q->end = (q->end + 1) % q->vector->capacity;
    }
    else {
        RETURN_ON_ERROR(vector_set(q->vector, q->end, element));
    }
    q->size++;

    return 0;
}

inline int queue_pop(struct queue *q, void *element) {
    assert(q);
    assert(element);

    if (q->size == 0) {
        return 1;
    }

    memcpy(element, 
           get_ptr_by_vector_index(q->vector, q->start), 
           q->vector->elemSize);
    q->start = (q->start + 1) % q->vector->capacity;
    q->size--;

    return 0;
}

int queue_empty(struct queue const *q) {
    assert(q);

    return q->size == 0;
}

struct queue *queue_delete(struct queue *q) {
    assert(q);

    vector_delete(q->vector);
    free(q);
    
    return NULL;
}

void queue_print(struct queue const *q, void (*pf)(void const *data)) {
    assert(q);
    assert(pf);

    if (q->size == 0) {
        printf("[]\n");
    }
    else {
        printf("[");

        size_t index = q->end;
        for (size_t nPrints = 0; nPrints < q->size - 1; ++nPrints) {            
            pf(get_ptr_by_vector_index(q->vector, index));
            printf(", ");

            index = (index + q->vector->capacity - 1) % q->vector->capacity;
        }
        pf(get_ptr_by_vector_index(q->vector, index));

        printf("]\n");
    }
}

static void print_double(void const *st) {
   printf("%lf", *(double *)st);
}

int main() {
    struct queue *q = queue_new(sizeof (double));
    for (int i = 0; i < 10; i++) {
        double tmp = i;
        queue_push(q, &tmp);
    }
    queue_print(q, print_double);

    for (int i = 0; i < 5; i++) {
        double tmp = i;
        queue_pop(q, &tmp);
    }
    queue_print(q, print_double);

    for (int i = 0; i < 3; i++) {
        double tmp = 10*i;
        queue_push(q, &tmp);
    }
    queue_print(q, print_double);

    while (!queue_empty(q)) {
        double tmp;
        queue_pop(q, &tmp);
        printf("%lf\n", tmp);
    }                                                                                                                                                                                        
 
    q = queue_delete(q);

//     struct queue *q = queue_new(sizeof (double));
//     for (int i = 1; i < 4; i++) {
//         double tmp = i;
//         queue_push(q, &tmp);
//     }
//     queue_print(q, print_double);
//     while (!queue_empty(q)) {
//         double tmp;
//         queue_pop(q, &tmp);
//         printf("%lf\n", tmp);
//     }                                                                                                                                                                                        
 
//     q = queue_delete(q);
}
// Будут выведены все числа от 0 до 9 включительно.