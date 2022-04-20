#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct list {
    char *key, *value;
    struct list *next;
};

// Создать новый узел со значением elem и вернуть указатель на него.
struct list *list_new(const char *key, const char *value) {
    struct list *head = (struct list*) calloc(1, sizeof(struct list));

    head->key = (char*) calloc(strlen(key) + 1, sizeof(char));
    strcpy(head->key, key);
    head->value = (char*) calloc(strlen(value) + 1, sizeof(char));
    strcpy(head->value, value);

    head->next = NULL;

    return head;
}

// Вставить в конец списка с головой head узел со значением elem. Если голова списка пуста — создать новый список. Вернуть голову нового списка.
struct list *list_insert(struct list *head, const char *key, const char *value) {
    if (head == NULL) {
        return list_new(key, value);
    }
    
    struct list* curNode = head;
    while (curNode->next) {
        curNode = curNode->next;
    }
    curNode->next = list_new(key, value);
    return head;
}

// Найти узел, начиная с головы списка, в котором значение будет равно elem и вернуть указатель на него. Если таких значений в списке нет, вернуть NULL.
struct list *list_find(struct list *head, const char *elem) {
    assert(head);

    while (strcmp(head->key, elem) && head->next) {
        head = head->next;
    }

    if (strcmp(head->key, elem) == 0) {
        return head;
    }
    return NULL;
}

void freeNode(struct list *node) {
    free(node->key);
    free(node->value);
    free(node);
}

// Найти узел, начиная с головы списка, в котором значение будет равно elem и удалить его из списка. Вернуть указатель на голову вновь получившегося списка.
struct list *list_erase(struct list *head, const char *elem) {
    assert(head);

    struct list* curNode = head;

    if (strcmp(head->key, elem) == 0) {
        curNode = head->next;
        freeNode(head);
        return curNode;
    }

    while (curNode->next && strcmp(curNode->next->key, elem)) {
        curNode = curNode->next;
    }
    if (curNode->next) {
        struct list* curNodeNext = curNode->next;
        curNode->next = curNode->next->next;
        freeNode(curNodeNext);
    } 

    return head;
}

// Удалить весь список, начинающийся с head. Вернуть NULL.
struct list *list_delete(struct list *head) {
    assert(head);

    struct list *curNode = head;

    while (head->next) {
        curNode = head;
        head = head->next;
        freeNode(curNode);
    }
    freeNode(head);

    return NULL;
}

// Вернуть указатель на следующий элемент списка. Если curr пустой — вернуть NULL.
struct list *list_next(struct list *curr) {
    if (curr) {
        return curr->next;
    }
    return NULL;
}

// Вывести на стандартный вывод содержимое списка в формате по образцу: []\n или [1, 2, 3]\n.
void list_print(struct list const *head) {
    if (head) {
        printf("[");
        while (head->next)  {
            printf("(%s, %s), ", head->key, head->value);
            head = head->next;
        }
        printf("(%s, %s)]\n", head->key, head->value);
    }
    else {
        printf("[]\n");
    }
}

//-----------------------------------------------------------------------------------

struct hashtable_t {
    size_t capacity, size;
    struct list **addrs;
};

inline size_t hash(const char *str) {
    assert(str);

    size_t ret = 0, strLen = strlen(str);
    for (size_t i = 0; i < strLen; i++) {
        ret *= 7;
        ret += str[i];
    }
    return ret;
}

struct hashtable_t *htCreate(size_t capacity) {
    struct hashtable_t *newHt = (struct hashtable_t*) calloc(1, sizeof(struct hashtable_t));
    newHt->capacity = capacity;
    newHt->size = 0;
    newHt->addrs = (struct list**) calloc(newHt->capacity, sizeof(struct list*));

    return newHt;
}

void htInsert(struct hashtable_t *ht, const char *key, const char *value) {
    assert(ht);
    assert(key);
    assert(value);

    size_t idx = hash(key) % ht->capacity;
    if (ht->addrs[idx]) {
        struct list *nodeToChange = list_find(ht->addrs[idx], key);
        if (nodeToChange) {
            nodeToChange->value = (char*) realloc(
                nodeToChange->value, (strlen(value) + 1) * sizeof(char));
            strcpy(nodeToChange->value, value);
        }
        else {
            list_insert(ht->addrs[idx], key, value);
        }
    }
    else {
        ht->addrs[idx] = list_new(key, value);
    }
}

void htErase(struct hashtable_t *ht, const char *key) {
    assert(ht);
    assert(key);

    size_t idx = hash(key) % ht->capacity;
    if (ht->addrs[idx]) {
        ht->addrs[idx] = list_erase(ht->addrs[idx], key);
    }
}

const char *htFind(struct hashtable_t *ht, const char *key) {
    assert(ht);
    assert(key);

    size_t idx = hash(key) % ht->capacity;
    if (ht->addrs[idx]) {
        struct list *found = list_find(ht->addrs[idx], key);
        if (found) {
            return found->value;
        }
    }
    return NULL;
}

void htDelete(struct hashtable_t *ht) {
    assert(ht);
    
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->addrs[i]) {
            list_delete(ht->addrs[i]);
        }
    }
    free(ht->addrs);
    free(ht);
}

void htPrint(struct hashtable_t *ht) {
    assert(ht);

    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->addrs[i]) {
            printf("%zu: ", i);
            list_print(ht->addrs[i]);
        }
    }
}

int main() {
    struct hashtable_t *hts[10] = {};
    int destroyed[10] = {};

    char buffer1[21] = {}, buffer2[21] = {};
    int n = 0;
    while (scanf("%s %d", buffer1, &n) != EOF) {
        if (destroyed[n]) {
            if (buffer1[0] == 'f') {
                printf("MISSING\n");
            }
            continue;
        }

        switch (buffer1[0]) {
        case 'c': {
            hts[n] = htCreate(1e5);
            break;
        }
        case 'i': {
            scanf("%s %s", buffer1, buffer2);
            htInsert(hts[n], buffer1, buffer2);
            break;
        }
        case 'e': {
            scanf("%s", buffer1);
            htErase(hts[n], buffer1);
            break;
        }
        case 'f': {
            scanf("%s", buffer1);
            const char *toPrint = htFind(hts[n], buffer1);
            if (toPrint) {
                printf("%s\n", toPrint);
            }
            else {
                printf("MISSING\n");
            }
            break;
        }
        case 'd': {
            destroyed[n] = 1;
            break;
        }
        }
    }

    for (int i = 0; i < 10; ++i) {
        if (hts[i]) {
            htDelete(hts[i]);
        }
    }
}