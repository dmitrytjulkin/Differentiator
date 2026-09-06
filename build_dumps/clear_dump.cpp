#include <stdio.h>
#include <assert.h>

#include "dump.h"

void ClearDump (const char* name_of_file)
{
    assert (name_of_file);

    FILE* output_ptr = fopen (name_of_file, "w");

    fclose (output_ptr);
}
