#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "tokens.h"

const int INIT_SIZE = 1000;
const int CAPACITY_ADDITION = 10;

void InitTokenArray (token_array_t* token_array)
{
    assert (token_array);

    token_array->data = (token_t*) calloc (INIT_SIZE, sizeof (token_t));
    assert (token_array->data);

    token_array->size = 0;
    token_array->capacity = INIT_SIZE;
}

void ResizeTokenArray (token_array_t* token_array)
{
    assert (token_array);

    token_array->capacity = token_array->capacity + CAPACITY_ADDITION;

    token_array->data = (token_t *) realloc (token_array->data, token_array->capacity * sizeof (token_t));
    assert (token_array->data);

    for (size_t i = token_array->size; i < token_array->capacity; ++i)
        token_array->data[i].type.num = 0;
}

void DestroyTokenArray (token_array_t* token_array)
{
    assert (token_array);

    free (token_array->data);
}

void PrintTokenArray (token_array_t* input_array)
{
    assert (input_array);
    printf ("PASSED in %s in %s, line = %d\n\n", __FILE__, __func__, __LINE__);

    printf ("the token array size = %zu\n", input_array->size);
    printf ("the token_array:\n");
    for (size_t i = 0; i < input_array->size; ++i)
        printf ("[%d] ", input_array->data[i].code);
    printf ("\n");
    for (size_t i = 0; i < input_array->size; ++i) {
        if (input_array->data[i].code == VAR_TOKEN)
            printf ("[ %s] ", input_array->data[i].type.var.name);

        else if (input_array->data[i].code == NUM_TOKEN)
            printf ("[ %d] ", input_array->data[i].type.num);

        else if (input_array->data[i].code == DIFF_VAR_TOKEN)
            printf ("[ %s]", input_array->data[i].type.var.name);

        else
            printf ("[ ] ");
    }
    printf ("\n");
}
