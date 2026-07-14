#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "headers/differentiator.h"
#include "headers/tokens.h"

const int EXTRA_SIZE = 10;

void SyntaxError (const char* funcname, int line);
void ResizeValIfNeed (char** val, size_t* val_size, size_t val_index);

node_t* GetAddOrSub (char* s, int* index);
node_t* GetMulOrDiv (char* s, int* index);
node_t* GetBrac     (char* s, int* index);
node_t* GetNum      (char* s, int* index);
node_t* GetVar      (char* s, int* index);
node_t* GetFunc     (char* s, int* index, node_t* var);
node_t* GetPow      (char* s, int* index);

tree_t* CreateTreeFromFile (FILE* input_ptr)
{
    assert (input_ptr);

    char* input_string = ReadInput (input_ptr);

    token_array_t* input_array = {};
    InitTokenArray (input_array);

    TokenizeInput (input_string, input_array);

    tree_t* tree = InitTree ();
    tree->root = GetExpression (input_array);

    fclose (input_ptr);
    DestroyTokenArray (input_array);

    return tree;
}

void SyntaxError (const char* funcname, int line)
{
    assert (funcname);

    printf ("SyntaxError was called from %s, line = %d\n\n", funcname, line);

    assert (0);
}

node_t* GetExpression (token_array_t* token_array)
{
    assert (token_array);

    int index = 0;

    node_t* node = GetAddOrSub (token_array, &index);

    if (index > token_array->size)
        SyntaxError (__func__, __LINE__);

    return node;
}

node_t* GetNum (token_array_t* token_array, int* index)
{
    assert (token_array);
    assert (index);

    int val = 0;

    if (token_array->data[*index].code != NUM_TOKEN)
        SyntaxError (__func__, __LINE__);

    val = token_array->data[*index++].data.val;
    ++*index;

    data_t tmp = {.num = 0};
    tmp.num = val;

    return NewNode (NUM, tmp, NULL, NULL);
}

node_t* GetAddOrSub (token_array_t* token_array, int* index)
{
    assert (token_array);
    assert (index);

    node_t* node = GetMulOrDiv (token_array, index);

    while (token_array->data[*index].code == ADD_TOKEN ||
           token_array->data[*index].code == SUB_TOKEN) {
        token_codes op = token_array[*index];

        ++*index;

        node_t* node2 = GetMulOrDiv (token_array, index);

        if (op == ADD_TOKEN)
            node = NewNode (OP, {.op = ADD}, node, node2);

        if (op == SUB_TOKEN)
            node = NewNode (OP, {.op = SUB}, node, node2);
    }

    return node;
}

node_t* GetMulOrDiv (token_array_t* token_array, int* index)
{
    assert (token_array);
    assert (index);

    node_t* node = GetPow (token_array, index);

    while (token_array->data[*index].code == MUL_TOKEN ||
           token_array->data[*index].code == SUB_TOKEN) {
        token_codes op = token_array->data[*index].code;

        ++*index;

        node_t* node2 = GetPow (token_array, index);

        if (op == MUL_TOKEN)
            node = NewNode (OP, {.op = MUL}, node, node2);

        if (op == SUB_TOKEN)
            node = NewNode (OP, {.op = DIV}, node, node2);
    }

    return node;
}

node_t* GetPow (token_array_t* token_array, int* index)
{
    assert (token_array);
    assert (index);

    node_t* node = GetBrac (token_array, index);

    while (token_array->data[*index].code == POW_TOKEN) {
        ++*index;

        node_t* node2 = GetBrac (token_array, index);

        node = NewNode (OP, {.op = POW}, node, node2);
    }

    return node;
}

node_t* GetBrac (token_array_t* token_array, int* index)
{
    assert (token_array);
    assert (index);

    node_t* node = InitNode ();

    if (token_array->data[*index].code == LEFT_BRACKET_TOKEN) {
        ++*index;

        node = GetAddOrSub (token_array, index);

        if (token_array->data[*index].code != RIGHT_BRACKET_TOKEN)
            SyntaxError (__func__, __LINE__);

        ++*index;

        return node;
    }

    if (token_array->data[*index].code == NUM_TOKEN)
        return GetNum (token_array, index);

    if (token_array->data[*index].code == VAR_TOKEN)
        return GetVar (token_array, index);

    return GetFunc (token_array, index, node);
}

node_t* GetVar (token_array_t* token_array, int* index)
{
    assert (token_array);
    assert (index);

    char* val = (char*) calloc (EXTRA_SIZE, sizeof (char));
    assert (val != NULL);

    size_t val_size = EXTRA_SIZE;
    size_t val_index = 0;

    if ('a' <= token_array[*index] && token_array[*index] <= 'z') {
        val[val_index++] = token_array[*index];

        ++*index;
    }

    while (('a' <= token_array[*index] && token_array[*index] <= 'z') ||
           ('0' <= token_array[*index] && token_array[*index] <= '9') || token_array[*index] == '_') {
        ResizeValIfNeed (&val, &val_size, val_index);

        val[val_index++] = token_array[*index];

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
    assert (val);
    assert (val_size);

    if (val_index == *val_size - 1) {
        val_size += EXTRA_SIZE;

        *val = (char*) realloc (*val, *val_size);
        assert (*val);
    }
}

node_t* GetFunc (char* s, int* index, node_t* node)
{
    assert (s);
    assert (index);
    assert (node);

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

