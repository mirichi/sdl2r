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


static VALUE sdl2r_font_im_dispose(VALUE self)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(self);

    TTF_CloseFont(fnt->font);
    fnt->font = 0;

    return self;
}


static VALUE sdl2r_font_im_get_disposed(VALUE self)
{
    struct SDL2RFont *fnt = SDL2R_GET_STRUCT(Font, self);
    return fnt->font ? Qfalse : Qtrue;
}


static VALUE sdl2r_ttf_close_font(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);

    TTF_CloseFont(fnt->font);
    fnt->font = 0;

    return vfont;
}


static VALUE sdl2r_ttf_init(VALUE klass)
{
    if (TTF_Init()) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return Qnil;
}


static VALUE sdl2r_ttf_quit(VALUE klass)
{
    TTF_Quit();
    return Qnil;
}


static VALUE sdl2r_ttf_open_font(VALUE klass, VALUE vfilename, VALUE vsize)
{
    VALUE vfont = sdl2r_font_alloc(cFont);
    struct SDL2RFont *fnt = SDL2R_GET_STRUCT(Font, vfont);

    SDL2R_RETRY(fnt->font = TTF_OpenFont(SDL2R_TO_UTF8_PTR(vfilename), NUM2INT(vsize)));
    if (!fnt->font) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vfont;
}


static VALUE sdl2r_ttf_open_font_index(VALUE klass, VALUE vfilename, VALUE vsize, VALUE vindex)
{
    VALUE vfont = sdl2r_font_alloc(cFont);
    struct SDL2RFont *fnt = SDL2R_GET_STRUCT(Font, vfont);

    SDL2R_RETRY(fnt->font = TTF_OpenFontIndex(SDL2R_TO_UTF8_PTR(vfilename), NUM2INT(vsize), NUM2INT(vindex)));
    if (!fnt->font) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vfont;
}


static VALUE sdl2r_ttf_render_solid(VALUE klass, VALUE vfont, VALUE vtext, VALUE vcolor)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    SDL_Color col;
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    SDL2R_SET_COLOR(col, vcolor);
    SDL2R_RETRY(sur->surface = TTF_RenderUTF8_Solid(fnt->font, SDL2R_TO_UTF8_PTR(vtext), col));
    if (!sur->surface) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vsurface;
}


static VALUE sdl2r_ttf_render_shaded(VALUE klass, VALUE vfont, VALUE vtext, VALUE vfgcolor, VALUE vbgcolor)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    SDL_Color fgcol, bgcol;
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    SDL2R_SET_COLOR(fgcol, vfgcolor);
    SDL2R_SET_COLOR(bgcol, vbgcolor);
    SDL2R_RETRY(sur->surface = TTF_RenderUTF8_Shaded(fnt->font, SDL2R_TO_UTF8_PTR(vtext), fgcol, bgcol));
    if (!sur->surface) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vsurface;
}


static VALUE sdl2r_ttf_render_blended(VALUE klass, VALUE vfont, VALUE vtext, VALUE vcolor)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    SDL_Color col;
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    SDL2R_SET_COLOR(col, vcolor);
    SDL2R_RETRY(sur->surface = TTF_RenderUTF8_Blended(fnt->font, SDL2R_TO_UTF8_PTR(vtext), col));
    if (!sur->surface) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return vsurface;
}


static VALUE sdl2r_ttf_get_font_outline(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_GetFontOutline(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_set_font_outline(VALUE klass, VALUE vfont, VALUE voutline)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);

    TTF_SetFontOutline(fnt->font, NUM2INT(voutline));
    return vfont;
}


static VALUE sdl2r_ttf_get_font_style(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_GetFontStyle(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_set_font_style(VALUE klass, VALUE vfont, VALUE vstyle)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);

    TTF_SetFontStyle(fnt->font, NUM2INT(vstyle));
    return vfont;
}


static VALUE sdl2r_ttf_get_font_hinting(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_GetFontHinting(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_set_font_hinting(VALUE klass, VALUE vfont, VALUE vhinting)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);

    TTF_SetFontHinting(fnt->font, NUM2INT(vhinting));
    return vfont;
}


static VALUE sdl2r_ttf_get_font_kerning(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_GetFontKerning(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_set_font_kerning(VALUE klass, VALUE vfont, VALUE vkerning)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);

    TTF_SetFontKerning(fnt->font, NUM2INT(vkerning));
    return vfont;
}


static VALUE sdl2r_ttf_font_height(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_FontHeight(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_font_ascent(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_FontAscent(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_font_descent(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_FontDescent(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_font_line_skip(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_FontLineSkip(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_font_faces(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_FontFaces(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_font_face_is_fixed_width(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_FontFaceIsFixedWidth(fnt->font);
    return INT2NUM(result);
}


static VALUE sdl2r_ttf_font_face_family_name(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    char *result;

    result = TTF_FontFaceFamilyName(fnt->font);
    return SDL2R_TO_UTF8_STRING(result);
}


static VALUE sdl2r_ttf_font_face_style_name(VALUE klass, VALUE vfont)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    char *result;

    result = TTF_FontFaceStyleName(fnt->font);
    return SDL2R_TO_UTF8_STRING(result);
}


static VALUE sdl2r_ttf_glyph_is_provided(VALUE klass, VALUE vfont, VALUE vstr)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;

    result = TTF_GlyphIsProvided(fnt->font, *(Uint16*)SDL2R_TO_UTF16_PTR(vstr));

    return INT2NUM(result);
}


static VALUE sdl2r_ttf_glyph_metrics(VALUE klass, VALUE vfont, VALUE vstr)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;
    int minx, maxx, miny, maxy, advance;
    Check_Type(vstr, T_STRING);

    result = TTF_GlyphMetrics(fnt->font, *(Uint16*)SDL2R_TO_UTF16_PTR(vstr)
                            , &minx, &maxx, &miny, &maxy, &advance);
    if (result == -1) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return rb_ary_new3(5, INT2NUM(minx), INT2NUM(maxx), INT2NUM(miny), INT2NUM(maxy), INT2NUM(advance));
}


static VALUE sdl2r_ttf_size(VALUE klass, VALUE vfont, VALUE vstr)
{
    struct SDL2RFont *fnt = SDL2R_GET_FONT_STRUCT(vfont);
    int result;
    int w, h;

    result = TTF_SizeUTF8(fnt->font, SDL2R_TO_UTF8_PTR(vstr), &w, &h);
    if (result == -1) {
        rb_raise(eSDLError, TTF_GetError());
    }

    return rb_ary_new3(2, INT2NUM(w), INT2NUM(h));
}


void Init_sdl2r_ttf(void)
{
    mTTF = rb_define_module_under(mSDL, "TTF");

    // TTF module methods
#define SDL2R_DEFINE_SINGLETON_METHOD_TTF(name, arity) rb_define_singleton_method(mTTF, #name, sdl2r_ttf_##name, arity)

    SDL2R_DEFINE_SINGLETON_METHOD_TTF(init, 0);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(quit, 0);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(open_font, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(open_font_index, 3);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(close_font, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(render_solid, 3);
//    SDL2R_DEFINE_SINGLETON_METHOD_TTF(render_utf8_solid, 3);
    rb_define_singleton_method(mTTF, "render_utf8_solid", sdl2r_ttf_render_solid, 3);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(render_shaded, 4);
//    SDL2R_DEFINE_SINGLETON_METHOD_TTF(render_utf8_shaded, 4);
    rb_define_singleton_method(mTTF, "render_utf8_shaded", sdl2r_ttf_render_shaded, 4);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(render_blended, 3);
//    SDL2R_DEFINE_SINGLETON_METHOD_TTF(render_utf8_blended, 3);
    rb_define_singleton_method(mTTF, "render_utf8_blended", sdl2r_ttf_render_blended, 3);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(get_font_outline, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(set_font_outline, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(get_font_style, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(set_font_style, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(get_font_hinting, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(set_font_hinting, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(get_font_kerning, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(set_font_kerning, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_height, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_ascent, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_descent, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_line_skip, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_faces, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_face_is_fixed_width, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_face_family_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(font_face_style_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(glyph_is_provided, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(glyph_metrics, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_TTF(size, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD_TTF(size_utf8, 2);
    rb_define_singleton_method(mTTF, "size_utf8", sdl2r_ttf_size, 3);

    // SDL::TTF::Font class
    cFont = rb_define_class_under(mTTF, "Font", rb_cObject);
    rb_define_alloc_func(cFont, sdl2r_font_alloc);

    rb_define_method(cFont, "dispose", sdl2r_font_im_dispose, 0);
    rb_define_method(cFont, "disposed?", sdl2r_font_im_get_disposed, 0);

#define SDL2R_DEFINE_CONST_TTF(t) rb_define_const(mTTF, #t, INT2NUM(TTF_##t))
    SDL2R_DEFINE_CONST_TTF(STYLE_BOLD);
    SDL2R_DEFINE_CONST_TTF(STYLE_ITALIC);
    SDL2R_DEFINE_CONST_TTF(STYLE_UNDERLINE);
    SDL2R_DEFINE_CONST_TTF(STYLE_STRIKETHROUGH);
    SDL2R_DEFINE_CONST_TTF(STYLE_NORMAL);
    SDL2R_DEFINE_CONST_TTF(HINTING_NORMAL);
    SDL2R_DEFINE_CONST_TTF(HINTING_LIGHT);
    SDL2R_DEFINE_CONST_TTF(HINTING_MONO);
    SDL2R_DEFINE_CONST_TTF(HINTING_NONE);
}
