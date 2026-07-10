#pragma once

struct var_t {
    char name[10];
    int val;
};

struct nametable_t {
    var_type* data;
    size_t size;
    size_t capacity;
};

void InitNametable    (nametable_t* nametable);
void ResizeNametable  (nametable_t* nametable);
void DestroyNametable (nametable_t* nametable);

