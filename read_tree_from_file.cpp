#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "tree.h"


void SyntaxError (const char* funcname, int line);
void ResizeValIfNeed (char** val, size_t* val_size, size_t val_index);

node_t* GetAddOrSub (char* s, int* index);
node_t* GetMulOrDiv (char* s, int* index);
node_t* GetBrac     (char* s, int* index);
node_t* GetNum      (char* s, int* index);
node_t* GetVar      (char* s, int* index);
node_t* GetFunc     (char* s, int* index, node_t* var);
node_t* GetPow      (char* s, int* index);

tree_t* CreateTreeFromFile ()
{
    FILE* input_ptr = fopen ("input.txt", "r");

    assert (input_ptr != NULL);

    char* input_array = ReadInput (input_ptr);

    assert (input_array != NULL);

    tree_t* tree = InitTree ();

    tree->root = GetExpression (input_array);

    fclose (input_ptr);
    free (input_array);

    return tree;
}

char* ReadInput (FILE* input)
{
    assert (input != NULL);

    struct stat input_data = {};
    fstat (fileno(input), &input_data);

    size_t size = (size_t) input_data.st_size;

    char* input_array = (char *) calloc (size + EXTRA_SIZE, sizeof(char));
    assert (input_array != NULL);

    fread (input_array, sizeof (char), size, input);

    return input_array;
}

void SyntaxError (const char* funcname, int line)
{
    assert (funcname != NULL);

    printf ("SyntaxError was called from %s, line = %d\n\n", funcname, line);

    assert (0);
}

node_t* GetExpression (char* s)
{
    assert (s != NULL);

    int index = 0;

    node_t* node = GetAddOrSub (s, &index);

    if (s[index] != '\0' && s[index] != '\n')
        SyntaxError (__func__, __LINE__);

    ++index;

    return node;
}

node_t* GetNum (char* s, int* index)
{
    assert (s != NULL);
    assert (index != NULL);

    int val = 0;

    while ('0' <= s[*index] && s[*index] <= '9') {
        val = 10 * val + (s[*index] - '0');

        ++*index;
    }

    data_t tmp = {.num = 0};
    tmp.num = val;

    return NewNode (NUM, tmp, NULL, NULL);
}

node_t* GetAddOrSub (char* s, int* index)
{
    assert (s != NULL);
    assert (index != NULL);

    node_t* node = GetMulOrDiv (s, index);

    while (s[*index] == '+' || s[*index] == '-') {
        int op = s[*index];

        ++*index;

        node_t* node2 = GetMulOrDiv (s, index);

        if (op == '+')
            node = NewNode (OP, {.op = ADD}, node, node2);

        if (op == '-')
            node = NewNode (OP, {.op = SUB}, node, node2);
    }

    return node;
}

node_t* GetMulOrDiv (char* s, int* index)
{
    assert (s != NULL);
    assert (index != NULL);

    node_t* node = GetPow (s, index);

    while (s[*index] == '*' || s[*index] == '/') {
        int op = s[*index];

        ++*index;

        node_t* node2 = GetPow (s, index);

        if (op == '*')
            node = NewNode (OP, {.op = MUL}, node, node2);

        if (op == '/')
            node = NewNode (OP, {.op = DIV}, node, node2);
    }

    return node;
}

node_t* GetPow (char* s, int* index)
{
    assert (s != NULL);
    assert (index != NULL);

    node_t* node = GetBrac (s, index);

    while (s[*index] == '^') {
        ++*index;

        node_t* node2 = GetBrac (s, index);

        node = NewNode (OP, {.op = POW}, node, node2);
    }

    return node;
}

node_t* GetBrac (char* s, int* index)
{
    assert (s != NULL);
    assert (index != NULL);

    node_t* node = InitNode ();

    if (s[*index] == '(') {
        ++*index;

        node = GetAddOrSub (s, index);

        if (s[*index] != ')')
            SyntaxError (__func__, __LINE__);

        ++*index;

        return node;
    }

    if ('0' <= s[*index] && s[*index] <= '9')
        return GetNum (s, index);

    if ('a' <= s[*index] && s[*index] <= 'z') {
        node = GetVar (s, index);

        node = GetFunc (s, index, node);
    }

    return node;
}

node_t* GetVar (char* s, int* index)
{
    assert (s != NULL);
    assert (index != NULL);

    char* val = (char*) calloc (EXTRA_SIZE, sizeof (char));
    assert (val != NULL);

    size_t val_size = EXTRA_SIZE;
    size_t val_index = 0;

    if ('a' <= s[*index] && s[*index] <= 'z') {
        val[val_index++] = s[*index];

        ++*index;
    }

    while (('a' <= s[*index] && s[*index] <= 'z') ||
           ('0' <= s[*index] && s[*index] <= '9') || s[*index] == '_') {
        ResizeValIfNeed (&val, &val_size, val_index);

        val[val_index++] = s[*index];

        ++*index;
    }

    data_t tmp = {.var = ""};
    strcpy (tmp.var, val);

    node_t* node = NewNode (VAR, tmp, NULL, NULL);

    free (val);

    return node;
}

void ResizeValIfNeed (char** val, size_t* val_size, size_t val_index)
{
    assert (val != NULL);
    assert (val_size != NULL);

    if (val_index == *val_size - 1) {
        val_size += EXTRA_SIZE;

        *val = (char*) realloc (*val, *val_size);
        assert (*val != NULL);
    }
}

node_t* GetFunc (char* s, int* index, node_t* node)
{
    assert (s != NULL);
    assert (index != NULL);
    assert (node != NULL);

    for (int i = 0; i < COUNT_OF_FUNC; ++i) {
        if (strcmp (node->data.var, list_of_func[i].name) == 0) {
            node->expr = FUNC;

            node->data.func = list_of_func[i].code;

            L = NULL;
            R = GetBrac (s, index);

            break;
        }
    }

    return node;
}

