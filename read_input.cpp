#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>

#include "headers/differentiator.h"

char* ReadInput (FILE* input)
{
    assert (input);

    struct stat input_data = {};
    fstat (fileno(input), &input_data);

    size_t size = (size_t) input_data.st_size;

    char* input_array = (char *) calloc (size + EXTRA_SIZE, sizeof(char));
    assert (input_array);

    fread (input_array, sizeof (char), size, input);

    return input_array;
}
