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

    if (rb_enc_get_index(vorg) != 0) {
        vorg = rb_str_export_to_enc(vorg, g_enc_utf8);
    }
    if (rb_enc_get_index(vapp) != 0) {
        vapp = rb_str_export_to_enc(vapp, g_enc_utf8);
    }

    result = SDL_GetPrefPath(RSTRING_PTR(vorg), RSTRING_PTR(vapp));
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


