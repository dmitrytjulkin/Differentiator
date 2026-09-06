#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "../parse_tree/tree.h"
#include "../work_with_token_array/tokens.h"
#include "read_input.h"

void ParseAfterExpression (token_array_t* input_array, nametable_t* dependencies,
                           token_array_t* arg_queue);
void SyntaxError (const char* funcname, int line);

node_t* GetExpression (token_array_t* token_array);
node_t* GetEquality (token_array_t* token_array, size_t* index);
node_t* GetAddOrSub (token_array_t* token_array, size_t* index);
node_t* GetMulOrDiv (token_array_t* token_array, size_t* index);
node_t* GetBrac     (token_array_t* token_array, size_t* index);
node_t* GetNum      (token_array_t* token_array, size_t* index);
node_t* GetVar      (token_array_t* token_array, size_t* index);
node_t* GetFunc     (token_array_t* token_array, size_t* index);
node_t* GetPow      (token_array_t* token_array, size_t* index);

tree_t* CreateTreeFromFile (FILE* input_ptr, nametable_t* dependencies,
                            token_array_t* arg_queue)
{
    assert (input_ptr);

    char* input_string = ReadInput (input_ptr);

    token_array_t input_array = {};
    InitTokenArray (&input_array);

    nametable_t nametable = {};
    InitNametable (&nametable);

    TokenizeInput (input_string, &input_array, &nametable);

    // PrintTokenArray (&input_array);

    tree_t* tree = InitTree ();
    tree->root = GetExpression (&input_array);

    ParseAfterExpression (&input_array, dependencies, arg_queue);

    fclose (input_ptr);
    DestroyTokenArray (&input_array);

    return tree;
}

void ParseAfterExpression (token_array_t* input_array,
                           nametable_t* dependencies, token_array_t* arg_queue)
{
    assert (input_array);
    assert (dependencies);
    assert (arg_queue);

    size_t index = 0;

    while (input_array->data[index].code != FINISH_TOKEN)
        ++index;
    ++index;

    while (input_array->data[index].code != FINISH_TOKEN)
        PasteToNametable (dependencies, input_array->data[index++].type.var.name);

    ++index;

    while (input_array->data[index].code != FINISH_TOKEN) {
        strcpy (arg_queue->data[arg_queue->size].type.var.name,
                input_array->data[index].type.var.name);

        ++arg_queue->size;
        ++index;

        if (arg_queue->capacity - arg_queue->size == 1)
            ResizeTokenArray (arg_queue);
    }
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

    size_t index = 0;

    node_t* node = GetEquality (token_array, &index);

    if (index > token_array->size)
        SyntaxError (__func__, __LINE__);

    return node;
}

node_t* GetEquality (token_array_t* token_array, size_t* index)
{
    assert (token_array);
    assert (index);

    node_t* left_part = GetAddOrSub (token_array, index);

    if (token_array->data[*index].code == EQUALITY_TOKEN) {
        ++*index;

        node_t* right_part = GetAddOrSub (token_array, index);

        return NewNode (OP, {.op = EQUALITY}, left_part, right_part);
    }

    return left_part;
}

node_t* GetAddOrSub (token_array_t* token_array, size_t* index)
{
    assert (token_array);
    assert (index);

    node_t* node = GetMulOrDiv (token_array, index);

    while (token_array->data[*index].code == ADD_TOKEN ||
           token_array->data[*index].code == SUB_TOKEN) {
        token_codes op = token_array->data[*index].code;

        ++*index;

        node_t* node2 = GetMulOrDiv (token_array, index);

        if (op == ADD_TOKEN)
            node = NewNode (OP, {.op = ADD}, node, node2);

        if (op == SUB_TOKEN)
            node = NewNode (OP, {.op = SUB}, node, node2);
    }

    return node;
}

node_t* GetMulOrDiv (token_array_t* token_array, size_t* index)
{
    assert (token_array);
    assert (index);

    node_t* node = GetPow (token_array, index);

    while (token_array->data[*index].code == MUL_TOKEN ||
           token_array->data[*index].code == DIV_TOKEN) {
        token_codes op = token_array->data[*index].code;

        ++*index;

        node_t* node2 = GetPow (token_array, index);

        if (op == MUL_TOKEN)
            node = NewNode (OP, {.op = MUL}, node, node2);

        if (op == DIV_TOKEN)
            node = NewNode (OP, {.op = DIV}, node, node2);
    }

    return node;
}

node_t* GetPow (token_array_t* token_array, size_t* index)
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

node_t* GetBrac (token_array_t* token_array, size_t* index)
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

    if (SQRT_TOKEN <= token_array->data[*index].code &&
        token_array->data[*index].code < NUM_TOKEN)
        return GetFunc (token_array, index);

    if (token_array->data[*index].code == FINISH_TOKEN)
        return NULL;

    assert (0);
    return NULL;
}

node_t* GetNum (token_array_t* token_array, size_t* index)
{
    assert (token_array);
    assert (index);

    int val = 0;

    if (token_array->data[*index].code != NUM_TOKEN)
        SyntaxError (__func__, __LINE__);

    val = token_array->data[*index].type.num;
    ++*index;

    data_t tmp = {.num = 0};
    tmp.num = val;

    return NewNode (NUM, tmp, NULL, NULL);
}

node_t* GetVar (token_array_t* token_array, size_t* index)
{
    assert (token_array);
    assert (index);

    char* val = token_array->data[*index].type.var.name;

    data_t tmp = {.var.name = ""};
    strcpy (tmp.var.name, val);
    ++*index;

    node_t* node = NewNode (VAR, tmp, NULL, NULL);

    return node;
}

node_t* GetFunc (token_array_t* token_array, size_t* index)
{
    assert (token_array);
    assert (index);

    data_t tmp = {.func = token_array->data[*index].type.func};

    ++*index;
    node_t* node = GetBrac (token_array, index);

    return NewNode (FUNC, tmp, NULL, node);
}
