struct var_type {
    char name[10];
    int val;
};

struct nametable_type {
    var_type* data;
    size_t size;
    size_t capacity;
};

void InitNametable (nametable_type* nametable);

