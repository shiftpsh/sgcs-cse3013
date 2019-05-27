#include "ordered_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL_NODE (&list->null_node)

static int __bst_leftRotate(ordered_list list, ordered_list_node x);

static int __bst_rightRotate(ordered_list list, ordered_list_node x);

static int __bst_transplant(ordered_list list, ordered_list_node u, ordered_list_node v);

static int __bst_insertRevalidate(ordered_list list, ordered_list_node u);

static int __bst_removeRevalidate(ordered_list list, ordered_list_node x);

static int __bst_freeNode(ordered_list_node t);

static int __bst_freeSublist(ordered_list_node x, ordered_list_node null);

static int __bst_nodeCompare(ordered_list_node a, ordered_list_node b);

ordered_list newList() {
    ordered_list list;

    list = (ordered_list) malloc(sizeof(struct __bst));
    list->root = NULL_NODE;
    list->null_node.parent = NULL_NODE;
    list->null_node.left = NULL_NODE;
    list->null_node.right = NULL_NODE;
    list->null_node.score = -1;
    list->null_node.size = 0;
    list->null_node.color = NULL_COLOR;
    list->total_order = 0;
    list->size = 0;
    return list;
}

int freeList(ordered_list list) {
    if (list != NULL) {
        __bst_freeSublist(list->root, NULL_NODE);
        free(list);
    }
    return 0;
}

ordered_list_node newEntry(int score, char *name) {
    ordered_list_node new_node;

    new_node = (ordered_list_node)malloc(sizeof(struct __bst_node));

    new_node->score = score;
    new_node->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(new_node->name, name);

    return new_node;
}

int orderedList_Insert(ordered_list list, ordered_list_node u) {
    ordered_list_node x;
    ordered_list_node y;

    u->order = list->total_order++;
    u->size = 1;

    x = list->root;
    y = NULL_NODE;
    while (x != NULL_NODE) {
        x->size++;
        y = x;
        if (__bst_nodeCompare(u, x)) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    u->parent = y;
    if (y == NULL_NODE) {
        list->root = u;
    } else if (__bst_nodeCompare(u, y)) {
        y->left = u;
    } else {
        y->right = u;
    }
    u->left = NULL_NODE;
    u->right = NULL_NODE;
    u->color = RED;
    __bst_insertRevalidate(list, u);
    list->size++;
    return 0;
}

int orderedList_Delete(ordered_list list, int r) {
    ordered_list_node x, y, u;
    int originalColor;

    if (!(1 <= r && r <= list->size)) {
        return -1;
    }

    u = orderedList_GetAt(list->root, r);

    y = u->parent;
    while (y != NULL_NODE) {
        y->size--;
        y = y->parent;
    }

    y = u;
    originalColor = y->color;
    if (u->left == NULL_NODE) {
        x = u->right;
        __bst_transplant(list, u, u->right);
    } else if (u->right == NULL_NODE) {
        x = u->left;
        __bst_transplant(list, u, u->left);
    } else {
        y = u->right;
        while (y->left != NULL_NODE) {
            y = y->left;
        }
        originalColor = y->color;
        x = y->right;
        if (y->parent == u) {
            x->parent = y;
        } else {
            __bst_transplant(list, y, y->right);
            y->right = u->right;
            y->right->parent = y;
        }
        __bst_transplant(list, u, y);
        y->left = u->left;
        y->left->parent = y;
        y->color = u->color;
        y->size = u->size - 1;
    }
    if (originalColor == BLACK) {
        __bst_removeRevalidate(list, x);
    }

    __bst_freeNode(u);
    list->size--;
    return 0;
}

ordered_list_node *orderedList_Query(ordered_list list, int l, int r) {
    ordered_list_node *nodes, *stack;
    int index, stx;

    ordered_list_node curr;

    if (!(1 <= l && l <= r && r <= list->size))
        return NULL;

    nodes = (ordered_list_node*)malloc(sizeof(ordered_list_node) * (r - l + 1));
    stack = (ordered_list_node*)malloc(sizeof(ordered_list_node) * (list->size));
    index = 0;
    stx = 0;
    curr = list->root;
    while (curr != NULL_NODE) {
        stack[stx++] = curr;
        curr = curr->left;
    }
    for (int i = 1; i <= r; i++) {
        curr = stack[--stx];
        if (l <= i) {
            nodes[index++] = curr;
        }
        curr = curr->right;
        while (curr != NULL_NODE) {
            stack[stx++] = curr;
            curr = curr->left;
        }
    }

    free(stack);
    return nodes;
}

int orderedList_indexOf(ordered_list list, ordered_list_node x) {
    int r;
    ordered_list_node y;
    if (list == NULL || x == NULL) {
        return -1;
    }

    r = x->left->size + 1;
    y = x;
    while (y != list->root) {
        if (y == y->parent->right) {
            r = r + y->parent->left->size + 1;
        }
        y = y->parent;
    }
    return r;
}

ordered_list_node orderedList_GetAt(ordered_list_node u, int i) {
    if (i < u->left->size) {
        return orderedList_GetAt(u->left, i);
    } else if (i > u->left->size) {
        return orderedList_GetAt(u->right, i - (u->left->size) - 1);
    } else {
        return u;
    }
}

static int __bst_leftRotate(ordered_list list, ordered_list_node x) {
    ordered_list_node y;

    if (x->right == NULL_NODE)
        return -1;

    y = x->right;
    x->size = x->left->size + y->left->size + 1;
    y->size = x->size + y->right->size + 1;

    x->right = y->left;
    if (y->left != NULL_NODE) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL_NODE) {
        list->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return 0;
}

static int __bst_rightRotate(ordered_list list, ordered_list_node x) {
    ordered_list_node y;

    if (x->left == NULL_NODE)
        return -1;

    y = x->left;

    x->size = y->right->size + x->right->size + 1;
    y->size = y->left->size + x->size + 1;

    x->left = y->right;
    if (y->right != NULL_NODE) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL_NODE) {
        list->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
    return 0;
}

static int __bst_transplant(ordered_list list, ordered_list_node u, ordered_list_node v) {
    if (u->parent == NULL_NODE) {
        list->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
    return 0;
}

static int __bst_insertRevalidate(ordered_list list, ordered_list_node u) {
    ordered_list_node y;

    while (u->parent->color == RED) {
        if (u->parent == u->parent->parent->left) {
            y = u->parent->parent->right;
            if (y->color == RED) {
                u->parent->color = BLACK;
                y->color = BLACK;
                u->parent->parent->color = RED;
                u = u->parent->parent;
            } else {
                if (u == u->parent->right) {
                    u = u->parent;
                    __bst_leftRotate(list, u);
                }
                u->parent->color = BLACK;
                u->parent->parent->color = RED;
                __bst_rightRotate(list, u->parent->parent);
            }
        } else {
            y = u->parent->parent->left;
            if (y->color == RED) {
                u->parent->color = BLACK;
                y->color = BLACK;
                u->parent->parent->color = RED;
                u = u->parent->parent;
            } else {
                if (u == u->parent->left) {
                    u = u->parent;
                    __bst_rightRotate(list, u);
                }
                u->parent->color = BLACK;
                u->parent->parent->color = RED;
                __bst_leftRotate(list, u->parent->parent);
            }
        }
    }
    list->root->color = BLACK;
    return 0;
}

static int __bst_removeRevalidate(ordered_list list, ordered_list_node x) {
    ordered_list_node w;
    while (x != list->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                __bst_leftRotate(list, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK, w->color = RED;
                    __bst_rightRotate(list, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                __bst_leftRotate(list, x->parent);
                x = list->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                __bst_rightRotate(list, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK,  w->color = RED;
                    __bst_leftRotate(list, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                __bst_rightRotate(list, x->parent);
                x = list->root;
            }
        }
    }
    x->color = BLACK;
    return 0;
}

static int __bst_freeNode(ordered_list_node u) {
    if (u->name != NULL) {
        free(u->name);
    }
    if (u != NULL) {
        free(u);
    }
    return 0;
}

static int __bst_freeSublist(ordered_list_node x, ordered_list_node null) {
    if (x == null)
        return 0;
    __bst_freeSublist(x->left, null);
    __bst_freeSublist(x->right, null);
    __bst_freeNode(x);
    return 0;
}

static int __bst_nodeCompare(ordered_list_node a, ordered_list_node b) {
    if (a->score > b->score)
        return 1;
    if (a->score < b->score)
        return 0;
    return a->order < b->order;
}
