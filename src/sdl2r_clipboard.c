#define SDL2RCLIPBOARD
#include "sdl2r.h"
#include "sdl2r_clipboard.h"


VALUE sdl2r_get_clipboard_text(VALUE klass)
{
    char *result;
    VALUE vresult;

    result = SDL_GetClipboardText();
    if (!result) {
        rb_raise(eSDLError, SDL_GetError());
    }

    vresult = SDL2R_TO_UTF8_STRING(result);
    SDL_free(result);

    return vresult;
}


VALUE sdl2r_has_clipboard_text(VALUE klass)
{
    SDL_bool result = SDL_HasClipboardText();
    return SDL2R_TO_BOOL(result);
}


VALUE sdl2r_set_clipboard_text(VALUE klass, VALUE vstr)
{
    int result = SDL_SetClipboardText(SDL2R_TO_UTF8_PTR(vstr));

    if (result) {
        rb_raise(eSDLError, SDL_GetError());
    }
    return vstr;
}


void Init_sdl2r_clipboard(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(get_clipboard_text, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(has_clipboard_text, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(set_clipboard_text, 1);
}


