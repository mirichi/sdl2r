#ifndef SDL2RHASH_H
#define SDL2RHASH_H

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
VALUE sdl2r_get_hash(struct SDL2RHash *h, HASHKEY key);
void sdl2r_put_hash(struct SDL2RHash *h, HASHKEY key, VALUE v);
void sdl2r_del_hash(struct SDL2RHash *h, HASHKEY key);

static const Uint8 __m_empty[]  = {0x02, 0x08, 0x20, 0x80};
static const Uint8 __m_del[]    = {0x01, 0x04, 0x10, 0x40};
static const Uint8 __m_either[] = {0x03, 0x0c, 0x30, 0xc0};

#define __ac_isempty(ed_flag, i) (ed_flag[(i)/4]&__m_empty[(i)%4])
#define __ac_isdel(ed_flag, i) (ed_flag[(i)/4]&__m_del[(i)%4])
#define __ac_iseither(ed_flag, i) (ed_flag[(i)/4]&__m_either[(i)%4])

#define sdl2r_hash_exist(h, x) (!__ac_iseither((h)->ed_flags, (x)))

#define sdl2r_hash_end(h) ((h)->n_buckets)

#define SDL2R_CLEAR_HASH(h, s, v, f) {\
        if (h) {\
            int k;\
            for (k = 0; k < sdl2r_hash_end(h); k++) {\
                if (sdl2r_hash_exist(h, k)) {\
                    struct SDL2R##s *tmp = SDL2R_GET_STRUCT(s, h->vals[k]);\
                    if (tmp->v) {\
                        f(tmp);\
                    }\
                }\
            }\
        }\
    }
#endif
