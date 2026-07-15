#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/nametable.h"

const int INIT_SIZE = 10;
const int CAPACITY_ADDITION = 10;

void InitNametable (nametable_t* nametable)
{
    assert (nametable);

    nametable->data = (var_t*) calloc (INIT_SIZE, sizeof (var_t));
    assert (nametable->data);

    nametable->size = 0;
    nametable->capacity = 0;
}

void ResizeNametable (nametable_t* nametable)
{
    assert (nametable);

    nametable->capacity = nametable->capacity + CAPACITY_ADDITION;

    nametable->data = (var_t *) realloc (nametable->data, nametable->capacity * sizeof (var_t));
    assert (nametable->data);

    for (size_t i = nametable->size; i < nametable->capacity; ++i) {
        strcpy (nametable->data[i].name, "");
        nametable->data[i].val = 0;
    }
}

void DestroyNametable (nametable_t* nametable)
{
    assert (nametable);

    free (nametable->data);
}
