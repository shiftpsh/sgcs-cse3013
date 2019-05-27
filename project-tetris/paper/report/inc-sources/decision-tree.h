#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__

typedef struct __score_pair {
    long double boardScore, propagatedScore;
} score_pair;

struct __decision_tree_node {
    score_pair score;
    char field[22][10];
    struct __decision_tree_node *parent;
    struct __decision_tree_node **children;
    int children_size;
    int curBlockID;
	int recBlockX, recBlockY, recBlockRotate;
};

typedef struct __decision_tree_node* decision_tree_node;

decision_tree_node decision_tree_create();

void decision_tree_destroy(decision_tree_node root);

void decision_tree_add_child(decision_tree_node node, decision_tree_node child);

void decision_tree_swap(decision_tree_node *a, decision_tree_node *b);

void decision_tree_node_quicksort(decision_tree_node *array, int start, int end);

long long decision_tree_size(decision_tree_node root);

#endif // __DECISION_TREE_H__
