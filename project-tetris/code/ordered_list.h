#ifndef __ORDERED_LIST_H__
#define __ORDERED_LIST_H__

enum __node_color { RED, BLACK, NULL_COLOR };

struct __bst_node {
    int score;
    int order;
    char *name;

    int size;
    enum __node_color color;
    struct __bst_node *parent;
    struct __bst_node *left;
    struct __bst_node *right;
};

typedef struct __bst_node* ordered_list_node;

struct __bst {
    ordered_list_node root;
    int size;
    unsigned int total_order;
    struct __bst_node null_node;
};

typedef struct __bst* ordered_list;

ordered_list newList();

int freeList(ordered_list list);

ordered_list_node newEntry(int score, char *name);

int orderedList_Insert(ordered_list list, ordered_list_node u);

int orderedList_Delete(ordered_list list, int r);

ordered_list_node *orderedList_Query(ordered_list list, int l, int r);

int orderedList_indexOf(ordered_list list, ordered_list_node x);

ordered_list_node orderedList_GetAt(ordered_list_node u, int i);

#endif // __ORDERED_LIST_H__
