#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"

node_t* d (node_t* node)
{
    if (node == NULL)
        return NULL;

    data_t cmd = {.func = ""};

    if (node->expr == NUM) {
        cmd.num = 0;

        return NewNode (NUM, cmd, NULL, NULL);
    }

    if (node->expr == VAR) {
        cmd.num = 1;

        return NewNode (NUM, (data_t) {.num = 1}, NULL, NULL);
    }

    if (node->expr == OP) {
        if (node->data.op[0] == '+' || node->data.op[0] == '-')
            return NewNode (OP, node->data, d(node->left), d(node->right));

        if (node->data.op[0] == '*')
            return NewNode (OP, {.op = "+"},
                            NewNode (OP, node->data, d (node->left), c (node->right)),
                            NewNode (OP, node->data, c (node->left), d (node->right)));

        if (node->data.op[0] == '/') {
            node_t* numer = NewNode (OP, {.op = "-"},
                                     NewNode (OP, {.op = "*"}, d (node->left), c (node->right)),
                                     NewNode (OP, {.op = "*"}, c (node->left), d (node->right)));

            node_t* denumer = NewNode (OP, {.op = "^"},
                                       c (node->right),
                                       NewNode (NUM, {.num = 2}, NULL, NULL));

            return NewNode (OP, node->data, numer, denumer);
        }

        if (node->data.op[0] == '^') {
            node_t* node1 = NewNode (OP, {.op = "*"},
                                    NewNode (FUNC, {.func = "ln"}, NULL, c (node->left)),
                                    c (node));

            node_t* frst_summand = NewNode (OP, {.op = "*"},
                                            node1,
                                            d (node->right));

            node_t* node2 = NewNode (OP, {.op = "^"},
                                    c (node->left),
                                    NewNode (OP, {.op = "-"},
                                             c (node->right),
                                             NewNode (NUM, {.num = 1}, NULL, NULL)));

            node_t* scnd_summand = NewNode (OP, {.op = "*"},
                                            NewNode (OP, {.op = "*"}, c (node->right), node2),
                                            d (node->left));

            return NewNode (OP, {.op = "+"}, frst_summand, scnd_summand);
        }
    }

    if (strcmp (node->data.func, "sqrt") == 0)
        return NewNode (OP, {.op = "*"},
                        NewNode (OP, {.op = "/"},
                                 NewNode (NUM, {.num = 0.5}, NULL, NULL),
                                 c (node)),
                        d (node->right));

    if (strcmp (node->data.func, "ln") == 0)
        return NewNode (OP, {.op = "*"},
                        NewNode (OP, {.op = "/"},
                                 NewNode (NUM, {.num = 1}, NULL, NULL),
                                 c (node->right)),
                        d (node->right));

    if (strcmp (node->data.func, "sin") == 0)
        return NewNode (OP, {.op = "*"},
                        NewNode (FUNC, {.func = "cos"}, NULL, c (node->right)),
                        d (node->right));

    if (strcmp (node->data.func, "cos") == 0)
        return NewNode (OP, {.op = "-"},
                        NewNode (NUM, {.num = 0}, NULL, NULL),
                        NewNode (OP, {.op = "*"},
                                 NewNode (FUNC, {.func = "sin"}, NULL, c (node->right)),
                                 d (node->right)));

    if (strcmp (node->data.func, "tg") == 0) {
        node_t* node1 = NewNode (OP, {.op = "/"},
                                 NewNode (NUM, {.num = 1}, NULL, NULL),
                                 NewNode (OP, {.op = "^"},
                                          NewNode (FUNC, {.func = "cos"}, NULL, c (node->right)),
                                          NewNode (NUM, {.num = 2}, NULL, NULL)));

        return NewNode (OP, {.op = "*"}, node1, d (node->right));
    }

    if (strcmp (node->data.func, "ctg") == 0) {
        node_t* node1 = NewNode (OP, {.op = "/"},
                                 NewNode (NUM, {.num = 1}, NULL, NULL),
                                 NewNode (OP, {.op = "^"},
                                          NewNode (FUNC, {.func = "sin"}, NULL, c (node->right)),
                                          NewNode (NUM, {.num = 2}, NULL, NULL)));

        return NewNode (OP, {.op = "-"},
                        NewNode (NUM, {.num = 0}, NULL, NULL),
                        NewNode (OP, {.op = "*"}, node1, d (node->right)));
    }

    if (strcmp (node->data.func, "arcsin") == 0) {
        node_t* node1 = NewNode (FUNC, {.func = "sqrt"},
                                 NULL,
                                 NewNode (OP, {.op = "-"},
                                          NewNode (NUM, {.num = 1}, NULL, NULL),
                                          NewNode (OP, {.op = "^"},
                                                   c (node->right),
                                                   NewNode (NUM, {.num = 2}, NULL, NULL))));

        return NewNode (OP, {.op = "*"},
                        NewNode (OP, {.op = "/"},
                                 NewNode (NUM, {.num = 1}, NULL, NULL),
                                 node1),
                        d (node->right));
    }

    return NULL;
}

node_t* c (node_t* node)
{
    if (node == NULL)
        return NULL;

    node_t* copy_node = InitNode ();

    copy_node->left = c (node->left);
    copy_node->right = c (node->right);

    copy_node->expr = node->expr;

    if (node->expr == NUM)
        copy_node->data.num = node->data.num;

    else
        strcpy (copy_node->data.func, node->data.func);

    return copy_node;
}
