#define SDL2RFILESYSTEM
#include "sdl2r.h"
#include "sdl2r_filesystem.h"

static VALUE sdl2r_get_base_path(VALUE klass)
{
    char *result;

    result = SDL_GetBasePath();
    if (!result) {
        rb_raise(eSDLError, "%s", SDL_GetError());
    }

    return SDL2R_TO_UTF8_STRING(result);
}


static VALUE sdl2r_get_pref_path(VALUE klass, VALUE vorg, VALUE vapp)
{
    char *result;

    result = SDL_GetPrefPath(SDL2R_TO_UTF8_PTR(vorg), SDL2R_TO_UTF8_PTR(vapp));
    if (!result) {
        rb_raise(eSDLError, "%s", SDL_GetError());
    }

    return SDL2R_TO_UTF8_STRING(result);
}


void Init_sdl2r_filesystem(void)
{
    SDL2R_DEFINE_SINGLETON_METHOD(get_base_path, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(get_pref_path, 2);
}
