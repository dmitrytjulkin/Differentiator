#pragma once

const int INIT_VAR_SIZE = 10;

struct var_t {
    char name[INIT_VAR_SIZE];
    int val;
};

struct nametable_t {
    var_t* data;
    size_t size;
    size_t capacity;
};

void InitNametable    (nametable_t* nametable);
void ResizeNametable  (nametable_t* nametable);
void DestroyNametable (nametable_t* nametable);

