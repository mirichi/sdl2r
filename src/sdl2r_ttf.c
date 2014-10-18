#include <SDL.h>

#define SDL2RTTF
#include "sdl2r.h"
#include "sdl2r_ttf.h"
#include "sdl2r_surface.h"

VALUE mTTF;
VALUE cFont;

static void sdl2r_font_free(void *ptr);

const rb_data_type_t sdl2r_font_data_type = {
    "Font",
    {
    0,
    sdl2r_font_free,
    0,
    },
};


static void sdl2r_font_free(void *ptr)
{
    struct SDL2RFont *fnt = ptr;
    if (fnt->font) {
        TTF_CloseFont(fnt->font);
        fnt->font = 0;
    }
    xfree(fnt);
}


VALUE sdl2r_font_alloc(VALUE klass)
{
    struct SDL2RFont *fnt;
    VALUE vfont = TypedData_Make_Struct(klass, struct SDL2RFont, &sdl2r_font_data_type, fnt);
    fnt->font = 0;
    return vfont;
}


static VALUE sdl2r_font_get_destroyed(VALUE self)
{
    struct SDL2RFont *fnt = SDL2R_GET_STRUCT(Font, self);
    return fnt->font ? Qfalse : Qtrue;
}


static VALUE sdl2r_ttf_init(VALUE klass)
{
    if (TTF_Init()) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return Qnil;
}


static VALUE sdl2r_ttf_open_font(VALUE klass, VALUE vfilename, VALUE vsize)
{
    VALUE vfont = sdl2r_font_alloc(cFont);
    struct SDL2RFont *fnt = SDL2R_GET_STRUCT(Font, vfont);

    Check_Type(vfilename, T_STRING);
    if (rb_enc_get_index(vfilename) != 0) {
        vfilename = rb_str_export_to_enc(vfilename, g_enc_utf8);
    }

    SDL2R_RETRY(fnt->font = TTF_OpenFont(RSTRING_PTR(vfilename), NUM2INT(vsize)));
    if (!fnt->font) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vfont;
}


static VALUE sdl2r_ttf_close_font(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);

    TTF_CloseFont(fnt->font);
    fnt->font = 0;

    return vfont;
}


static VALUE sdl2r_ttf_render_utf8_blended(VALUE klass, VALUE vfont, VALUE vtext, VALUE vcolor)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    SDL_Color col;
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    Check_Type(vtext, T_STRING);
    if (rb_enc_get_index(vtext) != 0) {
        vtext = rb_str_export_to_enc(vtext, g_enc_utf8);
    }

    Check_Type(vcolor, T_ARRAY);
    col.r = NUM2INT(rb_ary_entry(vcolor, 0));
    col.g = NUM2INT(rb_ary_entry(vcolor, 1));
    col.b = NUM2INT(rb_ary_entry(vcolor, 2));
    col.a = NUM2INT(rb_ary_entry(vcolor, 3));

    sur->surface = TTF_RenderUTF8_Blended(fnt->font, RSTRING_PTR(vtext), col);
    if (!sur->surface) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vsurface;
}


void Init_sdl2r_ttf(void)
{
    mTTF = rb_define_module_under(mSDL, "TTF");

    // TTF module methods
    rb_define_singleton_method(mTTF, "init", sdl2r_ttf_init, 0);
    rb_define_singleton_method(mTTF, "open_font", sdl2r_ttf_open_font, 2);
    rb_define_singleton_method(mTTF, "close_font", sdl2r_ttf_close_font, 1);
    rb_define_singleton_method(mTTF, "render_utf8_blended", sdl2r_ttf_render_utf8_blended, 3);

    // SDL::TTF::Font class
    cFont = rb_define_class_under(mTTF, "Font", rb_cObject);
    rb_define_alloc_func(cFont, sdl2r_font_alloc);

    rb_define_method(cFont, "destroyed?", sdl2r_font_get_destroyed, 0);
}


