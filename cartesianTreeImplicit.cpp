#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

typedef struct person {
    int salary;
    char name[31];
} person;

typedef struct node {
    size_t c, prior;
    node *left, *right;
    person data;
} node;

typedef struct pair {
    node *l, *r;
} pair;

inline size_t nChildren(node *t) {
    return t ? t->c : 0;
}

inline size_t updateC(node *t) {
    return nChildren(t->left) + nChildren(t->right) + 1;
}

node *merge(node *t1, node *t2) {
    if (t1 == NULL) {
        return t2;
    }
    else if (t2 == NULL) {
        return t1;
    }
    else if (t1->prior > t2->prior) {
        t1->right = merge(t1->right, t2);
        t1->c = updateC(t1);
        return t1;
    }
    else {
        t2->left = merge(t1, t2->left);
        t2->c = updateC(t2);
        return t2;
    }
}

pair split(node *t, size_t idx) {
    if (t == NULL) {
        return {NULL, NULL};
    }
    size_t l = nChildren(t->left);
    if (idx <= l) {
        pair p = split(t->left, idx);
        t->left = p.r;
        t->c = updateC(t);
        return {p.l, t};
    }
    else {
        pair p = split(t->right, idx - l - 1);
        t->right = p.l;
        t->c = updateC(t);
        return {t, p.r};
    }
}

node *find(node *t, size_t idx) {
    if (t == NULL || idx == nChildren(t->left)) {
        return t;
    }
    else if (idx <= nChildren(t->left)) {
        return find(t->left, idx);
    }
    else {
        return find(t->right, idx - nChildren(t->left) - 1);
    }
}

node *nodeCreate(person data) {
    node *newNode = (node*) calloc(1, sizeof(node));
    newNode->data = data;
    newNode->c = 1;
    newNode->prior = rand();
    newNode->left = newNode->right = NULL;

    return newNode;
}

node *insert(node *t, size_t idx, person data) {
    pair p = split(t, idx);
    return merge(p.l, merge(nodeCreate(data), p.r));
}

node *remove(node *t, size_t idx) {
    pair lrPair = split(t, idx);
    pair mrPair = split(lrPair.r, 1);
    free(mrPair.l);
    return merge(lrPair.l, mrPair.r);
}

void deleteCT(node *t) {
    if (t->left) {
        deleteCT(t->left);
    }
    if (t->right) {
        deleteCT(t->right);
    }
    free(t);
}

int main() {
    node *treap = NULL;

    size_t n = 0, m = 0;
    scanf("%zu %zu", &n, &m);

    for (size_t i = 0; i < n; ++i) {
        person p;
        scanf("%s %d", p.name, &p.salary);
        treap = insert(treap, i, p);
    }

    for (size_t i = 0; i < m; ++i) {
        size_t cmd = 0, idx = 0;
        scanf("%zu %zu", &cmd, &idx);

        switch (cmd)
        {
        case 0: {
            person p;
            scanf("%s %d", p.name, &p.salary);
            treap = insert(treap, idx - 1, p);
            break;
        }
        case 1: {
            treap = remove(treap, idx - 1);
            break;
        }
        case 2: {
            node *found = find(treap, idx - 1);
            printf("%s %d\n", found->data.name, found->data.salary);
            break;
        }
        default:
            break;
        }
    }

    deleteCT(treap);
}