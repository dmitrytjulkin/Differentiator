#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"

// int main ()
// {
//     char* s = "cos(sin(x))";
//
//     node_t* root = GetG (&s);
//
//     RunGraphDump (root, "graph_dump.dot",
//                   "dot -Tsvg graph_dump.dot -o graph_dump.svg");
// }

void SyntaxError (const char* funcname, int line)
{
    printf ("SyntaxError was called from %s, line = %d\n\n", funcname, line);

    assert (0);
}

node_t* GetG (char** s)
{
    assert (s != NULL);

    node_t* node = GetE (s);

    if (**s != '\0' && **s != '\n')
        SyntaxError (__func__, __LINE__);

    ++*s;

    return node;
}

node_t* GetN (char** s)
{
    assert (s != NULL);

    int val = 0;

    while ('0' <= **s && **s <= '9') {
        val = 10 * val + (**s - '0');

        ++*s;
    }

    data_t tmp = {.num = 0};
    tmp.num = val;

    return NewNode (NUM, tmp, NULL, NULL);
}

node_t* GetE (char** s)
{
    assert (s != NULL);

    node_t* node = GetT (s);

    while (**s == '+' || **s == '-') {
        int op = **s;

        ++*s;

        node_t* node2 = GetT (s);

        if (op == '+')
            node = NewNode (OP, {.op[0] = '+'}, node, node2);

        if (op == '-')
            node = NewNode (OP, {.op[0] = '-'}, node, node2);
    }

    return node;
}

node_t* GetT (char** s)
{
    assert (s != NULL);

    node_t* node = GetPow (s);

    while (**s == '*' || **s == '/') {
        int op = **s;

        ++*s;

        node_t* node2 = GetPow (s);

        if (op == '*')
            node = NewNode (OP, {.op[0] = '*'}, node, node2);

        if (op == '/')
            node = NewNode (OP, {.op[0] = '/'}, node, node2);
    }

    return node;
}

node_t* GetPow (char** s)
{
    assert (s != NULL);

    node_t* node = GetP (s);

    while (**s == '^') {
        ++*s;

        node_t* node2 = GetP (s);

        node = NewNode (OP, {.op[0] = '^'}, node, node2);
    }

    return node;
}

node_t* GetP (char** s)
{
    assert (s != NULL);

    node_t* node = InitNode ();

    if (**s == '(') {
        ++*s;

        node = GetE (s);

        if (**s != ')')
            SyntaxError (__func__, __LINE__);

        ++*s;

        return node;
    }

    if ('0' <= **s && **s <= '9')
        return GetN (s);

    if ('a' <= **s && **s <= 'z') {
        node = GetV (s);

        node = GetF (s, node);
    }

    return node;
}

node_t* GetV (char** s)
{
    assert (s != NULL);

    char val[100] = ""; // TODO calloc + realloc
    int val_index = 0;

    if ('a' <= **s && **s <= 'z') {
        val[val_index++] = **s;

        ++*s;
    }

    while (('a' <= **s && **s <= 'z') ||
           ('0' <= **s && **s <= '9') || **s == '_') {
        val[val_index++] = **s;

        ++*s;
    }

    data_t tmp = {.var = ""};
    strcpy (tmp.var, val);

    return NewNode (VAR, tmp, NULL, NULL);
}

node_t* GetF (char** s, node_t* node)
{
    assert (s != NULL);
    assert (node != NULL);

    for (int i = 0; i < COUNT_OF_FUNC; ++i) {
        if (strcmp (node->data.func, list_of_func[i]) == 0) {
            node->expr = FUNC;
            node->left = NULL;
            node->right = GetP (s);

            break;
        }
    }

    return node;
}

