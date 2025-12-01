#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "tree.h"

node_t* ChooseOptimizingAlgorithm (node_t* node, int* is_tree_changed);
node_t* OptimizeOpBetweenNum (node_t* node);
node_t* OptimizeIfNodeIsZero (node_t* node, int* is_tree_changed);
node_t* OptimizeIfNodeIsOne (node_t* node, int* is_tree_changed);

// TODO sin NUM...

node_t* Optimize (node_t* node)
{
    int is_tree_changed = false;

    node_t* improved_node = InitNode();

    improved_node = ChooseOptimizingAlgorithm (node, &is_tree_changed);

    while (is_tree_changed) {
        is_tree_changed = false;

        improved_node = ChooseOptimizingAlgorithm (improved_node, &is_tree_changed);
    }

    return improved_node;
}

node_t* ChooseOptimizingAlgorithm (node_t* node, int* is_tree_changed)
{
    if (node == NULL)
        return node;

    if (node->right == NULL)
        return node;

    if (node->left == NULL)
        return node;

    node->left = ChooseOptimizingAlgorithm (node->left, is_tree_changed);
    node->right = ChooseOptimizingAlgorithm (node->right, is_tree_changed);

    if (node->expr == OP && node->left->expr == NUM && node->right->expr == NUM) {
        *is_tree_changed = true;

        return OptimizeOpBetweenNum (node);
    }

    if ((node->left->expr == NUM && IsZero (node->left->data.num)) ||
        (node->right->expr == NUM && IsZero (node->right->data.num)))
        return OptimizeIfNodeIsZero (node, is_tree_changed);

    if ((node->left->expr == NUM && IsZero(node->left->data.num - 1)) ||
        (node->right->expr == NUM && IsZero(node->right->data.num - 1)))
        return OptimizeIfNodeIsOne (node, is_tree_changed);

    return node; // ~
}

node_t* OptimizeOpBetweenNum (node_t* node)
{
    assert (node != NULL);

    OPTIMIZE_NUM_OP_NUM (+);

    OPTIMIZE_NUM_OP_NUM (-);

    OPTIMIZE_NUM_OP_NUM (*);

    OPTIMIZE_NUM_OP_NUM (/);

    if (node->data.op[0] == '^') {
        node->expr = NUM;
        node->data.num = pow (DeleteNodeAndRetData (node, LEFT).num,
                            DeleteNodeAndRetData (node, RIGHT).num);

        return node;
    }

    return node;
}

node_t* OptimizeIfNodeIsZero (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (node->left != NULL);
    assert (node->right != NULL);

    if (node->data.op[0] == '+') {
        *is_tree_changed = true;

        if (node->right->expr == NUM && IsZero (node->right->data.num))
            return NewNode (node->left->expr, node->left->data,
                                c (node->left->left), c (node->left->right));

        else
            return NewNode (node->right->expr, node->right->data,
                                c (node->right->left), c (node->right->right));
    }

    if (node->data.op[0] == '-' && IsZero (node->right->data.num)) {
        *is_tree_changed = true;

        return NewNode (node->left->expr, node->left->data,
                            c (node->left->left), c (node->left->right));
    }

    if (node->data.op[0] == '*') {
        *is_tree_changed = true;

        return NewNode (NUM, {.num = 0}, NULL, NULL);
    }

    if (node->data.op[0] == '/' && node->left->expr == NUM && IsZero (node->left->data.num)) {
        *is_tree_changed = true;

        return NewNode (NUM, {.num = 0}, NULL, NULL);
    }

    if (node->data.op[0] == '^') {
        *is_tree_changed = true;

        if (node->right->expr == NUM && IsZero (node->right->data.num))
            return NewNode (NUM, {.num = 1}, NULL, NULL);

        else
            return NewNode (NUM, {.num = 0}, NULL, NULL);
    }

    return node;
}

node_t* OptimizeIfNodeIsOne (node_t* node, int* is_tree_changed)
{
    assert (node != NULL);
    assert (node->left != NULL);
    assert (node->right != NULL);

    if (node->data.op[0] == '*') {
        *is_tree_changed = true;

        if (node->right->expr == NUM && IsZero (node->right->data.num - 1))
            return NewNode (node->left->expr, node->left->data,
                                c (node->left->left), c (node->left->right));

        else
            return NewNode (node->right->expr, node->right->data,
                                c (node->right->left), c (node->right->right));
    }

    if (node->data.op[0] == '/' && node->right->expr == NUM && IsZero (node->right->data.num - 1)){
        *is_tree_changed = true;

        return NewNode (node->left->expr, node->left->data,
                            c (node->left->left), c (node->left->right));
    }

    if (node->data.op[0] == '^') {
        *is_tree_changed = true;

        if (node->left->expr == NUM && IsZero (node->left->data.num - 1))
            return NewNode (NUM, {.num = 1}, NULL, NULL);

        else
            return NewNode (node->left->expr, node->left->data,
                                c (node->left->left), c (node->left->right));
    }

    return node;
}
