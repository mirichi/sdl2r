#include <SDL.h>
#include "ruby.h"
#include "ruby/encoding.h"

#ifndef SDL2R
extern VALUE mSDL;
extern VALUE eSDLError;
extern VALUE eSDL2RError;
extern rb_encoding *g_enc_utf8;
extern rb_encoding *g_enc_utf16;
extern VALUE cColor;
#endif

#define SDL2R_GET_STRUCT(name, obj) ((struct SDL2R##name *)DATA_PTR(obj))
#define SDL2R_RETRY(test) {SDL_bool retry_flag = SDL_FALSE;do {\
                            if (!(test) && !retry_flag) {rb_gc_start();retry_flag = SDL_TRUE;}else{retry_flag = SDL_FALSE;}\
}                          while(retry_flag);}
#define SDL2R_DEFINE_CONST(c, t) rb_define_const(c, #t, INT2NUM(SDL_##t))

#define SDL2R_SET_RECT(rect, vrect) {\
            Check_Type(vrect, T_ARRAY);\
            rect.x = NUM2INT(rb_ary_entry(vrect, 0));\
            rect.y = NUM2INT(rb_ary_entry(vrect, 1));\
            rect.w = NUM2INT(rb_ary_entry(vrect, 2));\
            rect.h = NUM2INT(rb_ary_entry(vrect, 3));\
        }
#define SDL2R_SET_COLOR(color, vcolor) {\
            Check_Type(vcolor, T_ARRAY);\
            color.r = NUM2INT(rb_ary_entry(vcolor, 0));\
            color.g = NUM2INT(rb_ary_entry(vcolor, 1));\
            color.b = NUM2INT(rb_ary_entry(vcolor, 2));\
            color.a = NUM2INT(rb_ary_entry(vcolor, 3));\
        }
#define SDL2R_TO_UTF8_PTR(vstr) RSTRING_PTR(rb_str_export_to_enc(vstr, g_enc_utf8))
#define SDL2R_TO_UTF16_PTR(vstr) RSTRING_PTR(rb_str_export_to_enc(vstr, g_enc_utf16))
#define SDL2R_TO_UTF8_STRING(str) rb_enc_str_new(str, strlen(str), g_enc_utf8)

