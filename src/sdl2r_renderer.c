#include <SDL.h>

#define SDL2RRENDERER
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_window.h"
#include "sdl2r_renderer.h"
#include "sdl2r_texture.h"

VALUE cRenderer;

static void sdl2r_renderer_free(void *ptr);
static void sdl2r_renderer_mark(void *ptr);

const rb_data_type_t sdl2r_renderer_data_type = {
    "Renderer",
    {
    sdl2r_renderer_mark,
    sdl2r_renderer_free,
    0,
    },
};


static void sdl2r_renderer_free(void *ptr)
{
    struct SDL2RRenderer *ren = ptr;
    xfree(ren);
}


static void sdl2r_renderer_mark(void *ptr)
{
    struct SDL2RRenderer *ren = ptr;
    rb_gc_mark(ren->vwindow);
}


VALUE sdl2r_renderer_alloc(VALUE klass)
{
    struct SDL2RRenderer *ren;
    VALUE vren = TypedData_Make_Struct(klass, struct SDL2RRenderer, &sdl2r_renderer_data_type, ren);
    ren->vwindow = Qnil;
    return vren;
}


static VALUE sdl2r_render_copy(VALUE klass, VALUE vrenderer, VALUE vtexture, VALUE vsrcrect, VALUE vdstrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);
    SDL_Rect srcrect, dstrect;
    SDL_Rect *psr=0, *pdr=0;

    if (vsrcrect != Qnil) {
        Check_Type(vsrcrect, T_ARRAY);
        srcrect.x = NUM2INT(rb_ary_entry(vsrcrect, 0));
        srcrect.y = NUM2INT(rb_ary_entry(vsrcrect, 1));
        srcrect.w = NUM2INT(rb_ary_entry(vsrcrect, 2));
        srcrect.h = NUM2INT(rb_ary_entry(vsrcrect, 3));
        psr = &srcrect;
    }
    if (vdstrect != Qnil) {
        Check_Type(vdstrect, T_ARRAY);
        dstrect.x = NUM2INT(rb_ary_entry(vdstrect, 0));
        dstrect.y = NUM2INT(rb_ary_entry(vdstrect, 1));
        dstrect.w = NUM2INT(rb_ary_entry(vdstrect, 2));
        dstrect.h = NUM2INT(rb_ary_entry(vdstrect, 3));
        pdr = &dstrect;
    }

    if (SDL_RenderCopy(win->renderer, tex->texture, psr, pdr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_copy_ex(VALUE klass, VALUE vrenderer, VALUE vtexture, VALUE vsrcrect, VALUE vdstrect, VALUE vangle, VALUE vcenter, VALUE vflip)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);
    SDL_Rect srcrect, dstrect;
    SDL_Rect *psr=0, *pdr=0;
    SDL_Point point;
    SDL_Point *pp=0;

    if (vsrcrect != Qnil) {
        Check_Type(vsrcrect, T_ARRAY);
        srcrect.x = NUM2INT(rb_ary_entry(vsrcrect, 0));
        srcrect.y = NUM2INT(rb_ary_entry(vsrcrect, 1));
        srcrect.w = NUM2INT(rb_ary_entry(vsrcrect, 2));
        srcrect.h = NUM2INT(rb_ary_entry(vsrcrect, 3));
        psr = &srcrect;
    }
    if (vdstrect != Qnil) {
        Check_Type(vdstrect, T_ARRAY);
        dstrect.x = NUM2INT(rb_ary_entry(vdstrect, 0));
        dstrect.y = NUM2INT(rb_ary_entry(vdstrect, 1));
        dstrect.w = NUM2INT(rb_ary_entry(vdstrect, 2));
        dstrect.h = NUM2INT(rb_ary_entry(vdstrect, 3));
        pdr = &dstrect;
    }
    if (vcenter != Qnil) {
        Check_Type(vcenter, T_ARRAY);
        point.x = NUM2INT(rb_ary_entry(vcenter, 0));
        point.y = NUM2INT(rb_ary_entry(vcenter, 1));
        pp = &point;
    }

    if (SDL_RenderCopyEx(win->renderer, tex->texture, psr, pdr, NUM2DBL(vangle), pp, NUM2INT(vflip))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_present(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);

    SDL_RenderPresent(win->renderer);

    return vrenderer;
}


static VALUE sdl2r_destroy_renderer(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);

    SDL_DestroyRenderer(win->renderer);
    win->renderer = 0;

    return vrenderer;
}


static VALUE sdl2r_renderer_get_destroyed(VALUE self)
{
    struct SDL2RRenderer *ren = SDL2R_GET_STRUCT(Renderer, self);
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, ren->vwindow);

    return win->renderer ? Qfalse : Qtrue;
}


static VALUE sdl2r_create_texture_from_surface(VALUE klass, VALUE vrenderer, VALUE vsurface)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    VALUE vtexture = sdl2r_texture_alloc(cTexture);
    struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, vtexture);

    SDL2R_RETRY(tex->texture = SDL_CreateTextureFromSurface(SDL2R_GET_STRUCT(Window, ren->vwindow)->renderer, sur->surface));
    if (!tex->texture) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vtexture;
}


static VALUE sdl2r_render_clear(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);

    if (SDL_RenderClear(win->renderer)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_set_render_draw_color(VALUE klass, VALUE vrenderer, VALUE vr, VALUE vg, VALUE vb, VALUE va)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);

    if (SDL_SetRenderDrawColor(win->renderer, NUM2INT(vr), NUM2INT(vg), NUM2INT(vb), NUM2INT(va))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_create_texture(VALUE klass, VALUE vrenderer, VALUE vformat, VALUE vaccess, VALUE vw, VALUE vh)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);
    VALUE vtexture = sdl2r_texture_alloc(cTexture);
    struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, vtexture);

    SDL2R_RETRY(tex->texture = SDL_CreateTexture(win->renderer, NUM2UINT(vformat), NUM2INT(vaccess), NUM2INT(vw), NUM2INT(vh)));
    if (!tex->texture) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vtexture;
}


static VALUE sdl2r_set_render_target(VALUE klass, VALUE vrenderer, VALUE vtexture)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);
    SDL_Texture *t;

    if (vtexture == Qnil) {
        t = NULL;
    } else {
        t = SDL2R_GET_TEXTURE_STRUCT(vtexture)->texture;
    }

    if (SDL_SetRenderTarget(win->renderer, t)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_set_render_draw_blend_mode(VALUE klass, VALUE vrenderer, VALUE vblendmode)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);

    if (SDL_SetRenderDrawBlendMode(win->renderer, NUM2INT(vblendmode))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_get_render_draw_blend_mode(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);
    SDL_BlendMode bm;

    if (SDL_GetRenderDrawBlendMode(win->renderer, &bm)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(bm);
}


static VALUE sdl2r_render_fill_rect(VALUE klass, VALUE vrenderer, VALUE vrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(ren->vwindow);
    SDL_Rect rect;
    SDL_Rect *pr=0;

    if (vrect != Qnil) {
        Check_Type(vrect, T_ARRAY);
        rect.x = NUM2INT(rb_ary_entry(vrect, 0));
        rect.y = NUM2INT(rb_ary_entry(vrect, 1));
        rect.w = NUM2INT(rb_ary_entry(vrect, 2));
        rect.h = NUM2INT(rb_ary_entry(vrect, 3));
        pr = &rect;
    }

    if (SDL_RenderFillRect(win->renderer, pr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


void Init_sdl2r_renderer(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "create_texture_from_surface", sdl2r_create_texture_from_surface, 2);
    rb_define_singleton_method(mSDL, "render_copy", sdl2r_render_copy, 4);
    rb_define_singleton_method(mSDL, "render_copy_ex", sdl2r_render_copy_ex, 7);
    rb_define_singleton_method(mSDL, "render_present", sdl2r_render_present, 1);
    rb_define_singleton_method(mSDL, "destroy_renderer", sdl2r_destroy_renderer, 1);
    rb_define_singleton_method(mSDL, "render_clear", sdl2r_render_clear, 1);
    rb_define_singleton_method(mSDL, "set_render_draw_color", sdl2r_set_render_draw_color, 5);
    rb_define_singleton_method(mSDL, "create_texture", sdl2r_create_texture, 5);
    rb_define_singleton_method(mSDL, "set_render_target", sdl2r_set_render_target, 2);
    rb_define_singleton_method(mSDL, "set_render_draw_blend_mode", sdl2r_set_render_draw_blend_mode, 2);
    rb_define_singleton_method(mSDL, "get_render_draw_blend_mode", sdl2r_get_render_draw_blend_mode, 1);
    rb_define_singleton_method(mSDL, "render_fill_rect", sdl2r_render_fill_rect, 2);

    // SDL::Renderer class
    cRenderer = rb_define_class_under(mSDL, "Renderer", rb_cObject);
    rb_define_alloc_func(cRenderer, sdl2r_renderer_alloc);

    rb_define_method(cRenderer, "destroyed?", sdl2r_renderer_get_destroyed, 0);

    // Constants
    rb_define_const(mSDL, "RENDERER_SOFTWARE", INT2FIX(SDL_RENDERER_SOFTWARE));
    rb_define_const(mSDL, "RENDERER_ACCELERATED", INT2FIX(SDL_RENDERER_ACCELERATED));
    rb_define_const(mSDL, "RENDERER_PRESENTVSYNC", INT2FIX(SDL_RENDERER_PRESENTVSYNC));
    rb_define_const(mSDL, "RENDERER_TARGETTEXTURE", INT2FIX(SDL_RENDERER_TARGETTEXTURE));
    rb_define_const(mSDL, "TEXTUREACCESS_STATIC", INT2FIX(SDL_TEXTUREACCESS_STATIC));
    rb_define_const(mSDL, "TEXTUREACCESS_STREAMING", INT2FIX(SDL_TEXTUREACCESS_STREAMING));
    rb_define_const(mSDL, "TEXTUREACCESS_TARGET", INT2FIX(SDL_TEXTUREACCESS_TARGET));
    rb_define_const(mSDL, "FLIP_NONE", INT2FIX(SDL_FLIP_NONE));
    rb_define_const(mSDL, "FLIP_HORIZONTAL", INT2FIX(SDL_FLIP_HORIZONTAL));
    rb_define_const(mSDL, "FLIP_VERTICAL", INT2FIX(SDL_FLIP_VERTICAL));
}

