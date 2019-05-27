#include "decision_tree.h"
#include <stdlib.h>

decision_tree_node decision_tree_create() {
    decision_tree_node newNode = (decision_tree_node)malloc(sizeof(struct __decision_tree_node));
    newNode->parent = NULL;
    newNode->children = (decision_tree_node *)malloc(0);
    newNode->children_size = 0;
    return newNode;
}

void decision_tree_destroy(decision_tree_node root) {
    for (int i = 0; i < root->children_size; i++) {
        if (root->children[i] == NULL) continue;
        decision_tree_destroy(root->children[i]);
    }
    free(root);
}

void decision_tree_add_child(decision_tree_node node, decision_tree_node child) {
    node->children = (decision_tree_node *)realloc(node->children, sizeof(decision_tree_node) * (node->children_size + 1));
    node->children[node->children_size] = child;
    (node->children_size)++;
}

void decision_tree_node_swap(decision_tree_node *a, decision_tree_node *b) {
    decision_tree_node temp = *a;
    *a = *b;
    *b = temp;
}

void decision_tree_node_quicksort(decision_tree_node *array, int start, int end) {
    if (start >= end)
        return;

    int p = start, i = start, j = end;

    while (i < j) {
        while (array[i]->score.boardScore + array[i]->score.propagatedScore
                <= array[p]->score.boardScore + array[p]->score.propagatedScore && i < end) {
            i++;
        }
        while (array[j]->score.boardScore + array[j]->score.propagatedScore
                > array[p]->score.boardScore + array[p]->score.propagatedScore) {
            j--;
        }

        if (i < j) {
            decision_tree_node_swap(&array[i], &array[j]);
        }
    }

    decision_tree_node_swap(&array[p], &array[j]);

    decision_tree_node_quicksort(array, start, j - 1);
    decision_tree_node_quicksort(array, j + 1, end);
}

long long decision_tree_size(decision_tree_node root) {
    long long size = sizeof(*root);
    for (int i = 0; i < root->children_size; i++) {
        size += decision_tree_size(root->children[i]);
    }
    return size;
}