#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define RETURN_ON_ERROR(func) { int error = (func); if (error) return error; }

struct stack {
    size_t size, capacity, elemSize;
    void *array;
};

static inline void* get_ptr_by_vector_index(const struct stack *stack, size_t index);
struct stack *stack_new(size_t elemSize);
int stack_push(struct stack *stack, const void *element);
int stack_top(struct stack *stack, void *element);
int stack_pop(struct stack *stack, void *element);
int stack_empty(struct stack const *stack);
struct stack *stack_delete(struct stack *stack);
void stack_print(struct stack const *stack, void (*print_func)(void const *data));

static inline void* get_ptr_by_vector_index(const struct stack *stack, size_t index) {
    assert(stack);
 
    return (char*) stack->array + (stack->elemSize * index);
}

struct stack *stack_new(size_t elemSize) {
    struct stack *newStack = (struct stack*) calloc(1, sizeof(struct stack));

    newStack->size = newStack->capacity = 0;
    newStack->elemSize = elemSize;
    newStack->array = calloc(newStack->capacity, newStack->elemSize);

    return newStack;
}

int stack_push(struct stack *stack, const void *element) {
    assert(stack);
    assert(element);

    if (stack->size == stack->capacity) {
        stack->capacity = stack->capacity * 2 + 1;
        void *newPtr = realloc(stack->array, stack->elemSize * stack->capacity);
        if (newPtr) {
            stack->array = newPtr;
        }
        else {
            return 1;
        }
    }

    memcpy(get_ptr_by_vector_index(stack, stack->size++), element, stack->elemSize);
    
    return 0;
}

int stack_top(struct stack *stack, void *element) {
    assert(stack);
    assert(element);

    if (!stack->size) {
        return 1;
    }

    memcpy(element, get_ptr_by_vector_index(stack, stack->size - 1), stack->elemSize);   
    return 0;
}

int stack_pop(struct stack *stack, void *element) {
    assert(stack);
    assert(element);

    if (!stack->size) {
        return 1;
    }

    RETURN_ON_ERROR(stack_top(stack, element));
    stack->size--;
    // уменьшать размер через гистерезис

    return 0;
}

int stack_empty(struct stack const *stack) {
    return stack->size == 0;
}

struct stack *stack_delete(struct stack *stack) {
    assert(stack);

    free(stack->array);
    free(stack);

    return NULL;
}

void stack_print(struct stack const *stack, void (*print_func)(void const *stack)) {
    assert(stack);
    assert(print_func);

    printf("[");
    for (size_t idx = 0; stack->size && (idx < stack->size - 1); ++idx) {
        print_func(get_ptr_by_vector_index(stack, idx));
        printf(", ");
    }
    if (stack->size > 0) {
        print_func(get_ptr_by_vector_index(stack, stack->size - 1));
    }
    printf("]\n");
}

static void print_double(void const *st) {
    printf("%d", *(int *)st);
}

int main() {
    struct stack *st = stack_new(sizeof (int));
    printf("%d\n", stack_empty(st));
    int tmp = 0;
    for (int i = 0; i < 10; i++) {
        tmp = i * i;
        stack_push(st, &tmp);
    }
    printf("%d\n", stack_empty(st));

    for (int i = 0; i < 5; ++i) {
        stack_pop(st, &tmp);
        print_double(&tmp);
        printf(" ");
    }

    stack_print(st, print_double);
    st = stack_delete(st);
}

// Должно быть выведено [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]\n