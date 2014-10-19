#include "ruby.h"
#include "ruby/encoding.h"

#ifndef SDL2R
extern VALUE mSDL;
extern VALUE eSDLError;
extern VALUE eSDL2RError;
extern rb_encoding *g_enc_utf8;
extern VALUE cPoint;
extern VALUE cRect;
extern VALUE cColor;
#endif

#define SDL2R_GET_STRUCT(name, obj) ((struct SDL2R##name *)DATA_PTR(obj))
#define SDL2R_RETRY(test) {SDL_bool retry_flag = SDL_FALSE;do {\
                            if (!(test) && !retry_flag) {rb_gc_start();retry_flag = SDL_TRUE;}else{retry_flag = SDL_FALSE;}\
}                          while(retry_flag);}
