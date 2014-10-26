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

    vresult = rb_enc_str_new(result, strlen(result), g_enc_utf8);
    SDL_free(result);

    return vresult;
}


VALUE sdl2r_has_clipboard_text(VALUE klass)
{
    return SDL_HasClipboardText() ? Qtrue : Qfalse;
}


VALUE sdl2r_set_clipboard_text(VALUE klass, VALUE vstr)
{
    Check_Type(vstr, T_STRING);
    if (rb_enc_get_index(vstr) != 0) {
        vstr = rb_str_export_to_enc(vstr, g_enc_utf8);
    }

    if (SDL_SetClipboardText(RSTRING_PTR(vstr))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vstr;
}


void Init_sdl2r_clipboard(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "get_clipboard_text", sdl2r_get_clipboard_text, 0);
    rb_define_singleton_method(mSDL, "has_clipboard_text", sdl2r_has_clipboard_text, 0);
    rb_define_singleton_method(mSDL, "set_clipboard_text", sdl2r_set_clipboard_text, 1);
}


