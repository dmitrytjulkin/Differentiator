#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/nametable.h"

const int INIT_CAPACITY = 1;
const int CAPACITY_ADDITION = 10;

void InitNametable (nametable_t* nametable)
{
    assert (nametable);

    nametable->data = (var_t*) calloc (INIT_CAPACITY, sizeof (var_t));
    assert (nametable->data);

    nametable->size = 0;
    nametable->capacity = INIT_CAPACITY;
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

void PasteToNametable (nametable_t* nametable, const char* var)
{
    assert (nametable);
    assert (var);

    size_t index = 0;

    while (index < nametable->size) {
        if (strcmp (nametable->data[index].name, var) == 0)
            return;

        ++index;
    }

    strcpy (nametable->data[nametable->size].name, var);
    ++nametable->size;

    if (nametable->capacity - nametable->size <= 2)
        ResizeNametable (nametable);
}

void PrintNametable (nametable_t* nametable)
{
    assert (nametable);

    printf ("\nNametable:\n");
    printf ("its size = %zu\n", nametable->size);
    for (size_t i = 0; i < nametable->size; ++i)
        printf ("[%s] ", nametable->data[i].name);

    printf ("\n\n");
}
