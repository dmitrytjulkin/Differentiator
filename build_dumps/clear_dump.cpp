#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>

#include "dump.h"

void ClearDump (const char* name_of_dir, const char* path_to_file)
{
    assert (path_to_file);
    assert (name_of_dir);

    struct stat st;

    if (stat(name_of_dir, &st) != 0 || !S_ISDIR(st.st_mode))
        mkdir (name_of_dir, 0755);

    FILE* output_ptr = fopen (path_to_file, "w");

    fclose (output_ptr);
}


