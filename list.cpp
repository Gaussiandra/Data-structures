#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct list {
    int value;
    struct list *next;
};

// Создать новый узел со значением elem и вернуть указатель на него.
struct list *list_new(int elem) {
    struct list *head = (struct list*) calloc(1, sizeof(struct list));
    head->value = elem;
    head->next = NULL;

    return head;
}

// Вставить в конец списка с головой head узел со значением elem. Если голова списка пуста — создать новый список. Вернуть голову нового списка.
struct list *list_insert(struct list *head, int elem) {
    if (head == NULL) {
        return list_new(elem);
    }
    
    struct list* curNode = head;
    while (curNode->next) {
        curNode = curNode->next;
    }
    curNode->next = list_new(elem);
    return head;
}

// Найти узел, начиная с головы списка, в котором значение будет равно elem и вернуть указатель на него. Если таких значений в списке нет, вернуть NULL.
struct list *list_find(struct list *head, int elem) {
    assert(head);

    while (head->value != elem && head->next) {
        head = head->next;
    }

    if (head->value == elem) {
        return head;
    }
    return NULL;
}

// Найти узел, начиная с головы списка, в котором значение будет равно elem и удалить его из списка. Вернуть указатель на голову вновь получившегося списка.
struct list *list_erase(struct list *head, int elem) {
    assert(head);

    struct list* curNode = head;

    if (head->value == elem) {
        curNode = head->next;
        free(head);
        return curNode;
    }

    while (curNode->next && curNode->next->value != elem) {
        curNode = curNode->next;
    }
    if (curNode->next) {
        struct list* curNodeNext = curNode->next;
        curNode->next = curNode->next->next;
        free(curNodeNext);
    } 

    return head;
}

// Вставить узел what после узла where в списке, готова которого находится в head. Вернуть указатель на голову нового списка. Вставка в пустой список приводит к созданию непустого. Вставка NULL или после NULL есть пустая операция.
struct list *list_insert_after(struct list *head, struct list *where, struct list *what) {
    if (where == NULL || what == NULL) {
        return head;
    }

    if (head == NULL) {
        return what;
    }

    if (where->next) {
        what->next = where->next;
    }
    where->next = what;

    return head;
}

// Вставить узел what перед узлом where в списке, готова которого находится в head. Вернуть указатель на голову нового списка. Вставка в пустой список приводит к созданию непустого. Вставка NULL или после NULL есть пустая операция.
struct list *list_insert_before(struct list *head, struct list *where, struct list *what) {
    if (where == NULL || what == NULL) {
        return head;
    }

    if (head == NULL) {
        return what;
    }

    if (head == where) {
        what->next = head;
        return what;
    }
    else { 
        struct list *curNode = head;
        while (curNode && curNode->next != where) {
            curNode = curNode->next;
        }
        curNode->next = what;
        what->next = where;
        return head;
    }
}

// Удалить весь список, начинающийся с head. Вернуть NULL.
struct list *list_delete(struct list *head) {
    assert(head);

    struct list *curNode = head;

    while (head->next) {
        curNode = head;
        head = head->next;
        free(curNode);
    }
    free(head);

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
            printf("%d, ", head->value);
            head = head->next;
        }
        printf("%d]\n", head->value);
    }
    else {
        printf("[]\n");
    }
}

int main() {
    struct list *head = list_new(0);
    
    head = list_insert(head, 1);
    head = list_insert(head, 2);
    head = list_insert(head, 3);
    head = list_insert(head, 4);
    head = list_insert(head, 5);
    head = list_insert(head, 6);
    head = list_insert(head, 7);
    head = list_insert(head, 8);
    head = list_insert(head, 9);
    head = list_insert(head, 10);

    list_find(head, 1);
    list_find(head, 10);
    list_find(head, 0);
    list_find(head, 20);

    list_print(head);

    head = list_erase(head, 5);
    head = list_erase(head, 5);
    head = list_erase(head, 10);
    head = list_erase(head, 0);

    list_print(head);

    head = list_delete(head);
    list_print(head);
}