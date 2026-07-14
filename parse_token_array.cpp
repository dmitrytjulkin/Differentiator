#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "headers/tokens.h"

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

    for (size_t i = token_array->size; i < token_array->capacity; ++i) {
        strcpy (token_array->data[i].name, "");
        token_array->data[i].val = 0;
    }
}

void DestroyTokenArray (token_array_t* token_array)
{
    assert (token_array);

    free (token_array->data);
}
