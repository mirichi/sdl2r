// mruby khash base. https://github.com/mruby/mruby

#define SDL2RHASH
#include "sdl2r.h"
#include "sdl2r_hash.h"

void sdl2r_hash_alloc_internal(struct SDL2RHash *h)
{
    int size = h->n_buckets;

    h->size = h->n_occupied = 0;
    h->keys = (HASHKEY *)ALLOC_N(Uint8, sizeof(HASHKEY) * size * 2 + sizeof(Uint8) * size / 4);
    h->vals = h->keys + size;
    h->ed_flags = (Uint8 *)(h->vals + size);
    memset(h->ed_flags, 0xaa, size / 4);
}


struct SDL2RHash *sdl2r_hash_alloc(int size)
{
    struct SDL2RHash *h = (struct SDL2RHash *)ALLOC(struct SDL2RHash);

    h->n_buckets = size;
    sdl2r_hash_alloc_internal(h);
    return h;
}


void sdl2r_destroy_hash(struct SDL2RHash *h)
{
    xfree(h->keys);
    xfree(h);
}


int sdl2r_get_hash(struct SDL2RHash *h, HASHKEY key)
{
    int k = (key >> 2) & (h->n_buckets - 1);
    int step = 0;

    while (!__ac_isempty(h->ed_flags, k)) {
        if (!__ac_isdel(h->ed_flags, k)) {
            if (h->keys[k] == key) return k;
        }
        k = (k+(++step)) & (h->n_buckets - 1);
    }
    return h->n_buckets;
}


void sdl2r_resize_hash(struct SDL2RHash *h, int new_n_buckets)
{
    Uint8 *old_ed_flags = h->ed_flags;
    HASHKEY *old_keys = h->keys;
    VALUE *old_vals = h->vals;
    int old_n_buckets = h->n_buckets;
    int i;

    h->n_buckets = new_n_buckets;
    sdl2r_hash_alloc_internal(h);
    for (i=0 ; i<old_n_buckets ; i++) {
        if (!__ac_iseither(old_ed_flags, i)) {
            int k = sdl2r_put_hash(h, old_keys[i], NULL);
            h->vals[k] = old_vals[i];
        }
    }
    xfree(old_keys);
}


int sdl2r_put_hash(struct SDL2RHash *h, HASHKEY key, int *ret)
{
    int k, del_k, step = 0;

    if (h->n_occupied >= h->n_buckets/2 + h->n_buckets/4) {
        sdl2r_resize_hash(h, h->n_buckets*2);
    }

    k = (key >> 2) & (h->n_buckets - 1);
    del_k = h->n_buckets;
    while (!__ac_isempty(h->ed_flags, k)) {
        if (!__ac_isdel(h->ed_flags, k)) {
            if (h->keys[k] == key) {
                if (ret) *ret = 0;
                return k;
            }
        }
        else if (del_k == h->n_buckets) {
            del_k = k;
        }
        k = (k+(++step)) & (h->n_buckets - 1);
    }
    if (del_k != h->n_buckets) {
        h->keys[del_k] = key;
        h->ed_flags[del_k/4] &= ~__m_del[del_k%4];
        h->size++;
        if (ret) *ret = 2;
        return del_k;
    } else {
        h->keys[k] = key;
        h->ed_flags[k/4] &= ~__m_empty[k%4];
        h->size++;
        h->n_occupied++;
        if (ret) *ret = 1;
        return k;
    }
}


void sdl2r_del_hash(struct SDL2RHash *h, int x)
{
    h->ed_flags[x/4] |= __m_del[x%4];
    h->size--;
}
