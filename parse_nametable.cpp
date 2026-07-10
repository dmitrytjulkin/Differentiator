#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "headers/nametable.h"

void InitNametable (nametable_type* nametable)
{
    assert (nametable);

    nametable->data = (var_type*) calloc (1, sizeof (var_type))
    assert (nametable->data);

    nametable->size = 0;
    nametable->capacity = 0;
}

void ResizeNametable (nametable_type* nametable)
{
    assert (nametable);

    nametable->capacity *= 2;

    nametable->array = (var_type *) realloc (nametable->array, nametable->capacity * sizeof (var_type));`
    assert (nametable->array);

    for (size_t i = nametable->size; i < nametable->capacity; ++i) {
        strcpy (nametable->array[i].name, "");
        nametable->array[i].val = 0;
    }
}

void DestroyNametable (nametable_type* nametable)
{
    assert (nametable != NULL);

    free (nametable->array);
}
