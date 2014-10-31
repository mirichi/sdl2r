#define SDL2RFILESYSTEM
#include "sdl2r.h"
#include "sdl2r_filesystem.h"

static VALUE sdl2r_get_base_path(VALUE klass)
{
    char *result;

    result = SDL_GetBasePath();
    if (!result) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return rb_enc_str_new(result, strlen(result), g_enc_utf8);
}


static VALUE sdl2r_get_pref_path(VALUE klass, VALUE vorg, VALUE vapp)
{
    char *result;

    result = SDL_GetPrefPath(SDL2R_TO_UTF8_PTR(vorg), SDL2R_TO_UTF8_PTR(vapp));
    if (!result) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return rb_enc_str_new(result, strlen(result), g_enc_utf8);
}


void Init_sdl2r_filesystem(void)
{
    rb_define_singleton_method(mSDL, "get_base_path", sdl2r_get_base_path, 0);
    rb_define_singleton_method(mSDL, "get_pref_path", sdl2r_get_pref_path, 2);
}


