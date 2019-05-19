#ifndef __ORDERED_LIST_H__
#define __ORDERED_LIST_H__

enum __node_color { RED, BLACK, NULL_COLOR };

typedef struct __node {
    int score;
    int order;
    char *name;

    int size;
    enum __node_color color;
    struct __node *parent;
    struct __node *left;
    struct __node *right;
} node;

typedef struct __bst {
    node *root;
    int size;
    unsigned int total_order;
    node null_node;
} ordered_list;

ordered_list *newList();

int freeList(ordered_list *list);

node *newEntry(int score, char *name);

int orderedList_Insert(ordered_list *list, node *u);

int orderedList_Delete(ordered_list *list, int r);

node **orderedList_Query(ordered_list *list, int l, int r);

int orderedList_indexOf(ordered_list *list, node *x);

node *orderedList_GetAt(node *u, int i);

#endif // __ORDERED_LIST_H__
