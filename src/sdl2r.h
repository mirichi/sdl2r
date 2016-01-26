#include "ruby.h"
#include "SDL.h"
#include "ruby/encoding.h"

#ifndef SDL2R
extern VALUE mSDL;
extern VALUE eSDLError;
extern VALUE eSDL2RError;
extern rb_encoding *g_enc_utf8;
extern rb_encoding *g_enc_utf16;
extern VALUE cColor;
extern VALUE g_enum_array;
#endif

#define SDL2R_GET_STRUCT(name, obj) ((struct SDL2R##name *)DATA_PTR(obj))
#define SDL2R_RETRY(test) {SDL_bool retry_flag = SDL_FALSE;do {\
                            if (!(test) && !retry_flag) {rb_gc_start();retry_flag = SDL_TRUE;}else{retry_flag = SDL_FALSE;}\
}                          while(retry_flag);}
#define SDL2R_DEFINE_CONST(t) rb_define_const(mSDL, #t, INT2NUM(SDL_##t))
#define SDL2R_DEFINE_CONST_N(t) rb_define_const(mSDL, #t, INT2NUM(t))
#define SDL2R_DEFINE_SINGLETON_METHOD(name, arity) rb_define_singleton_method(mSDL, #name, sdl2r_##name, arity)
#define SDL2R_DEFINE_SINGLETON_METHOD_MACRO(name, arity) rb_define_singleton_method(mSDL, #name, sdl2r_macro_##name, arity)
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
#define SDL2R_TO_BOOL(b) ((b) ? Qtrue : Qfalse)

#define SDL2R_DEFINE_ENUM(t) {sdl2r_##t = rb_hash_new();rb_ary_push(g_enum_array, sdl2r_##t);rb_define_const(mSDL, #t, sdl2r_##t);}
#define SDL2R_DEFINE_ENUM_VALUE(t, v) {rb_hash_aset(sdl2r_##t, INT2NUM(SDL_##v), rb_str_new(#v, strlen(#v)));rb_define_const(mSDL, #v, INT2NUM(SDL_##v));}
#define SDL2R_DEFINE_ENUM_VALUE_N(t, v) {rb_hash_aset(sdl2r_##t, INT2NUM(v), rb_str_new(#v, strlen(#v)));rb_define_const(mSDL, #v, INT2NUM(v));}
