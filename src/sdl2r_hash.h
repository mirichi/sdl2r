#define HASHKEY VALUE

struct SDL2RHash {
    int n_buckets;
    int size;
    int n_occupied;
    HASHKEY *keys;
    VALUE *vals;
    Uint8 *ed_flags;
};

struct SDL2RHash *sdl2r_hash_alloc(int size);
void sdl2r_destroy_hash(struct SDL2RHash *h);
int sdl2r_get_hash(struct SDL2RHash *h, HASHKEY key);
int sdl2r_put_hash(struct SDL2RHash *h, HASHKEY key, int *ret);
void sdl2r_del_hash(struct SDL2RHash *h, int x);
