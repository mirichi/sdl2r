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


void sdl2r_dispose_renderer(struct SDL2RRenderer *ren)
{
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, ren->vwindow);
    int k;
    SDL_DestroyRenderer(ren->renderer);
    ren->renderer = 0;
    ren->vwindow = Qnil;
    win->vrenderer = Qnil;
    for (k = 0; k < ren->th->n_buckets; k++) {
        if (sdl2r_hash_exist(ren->th, k)) {
            struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, ren->th->vals[k]);
            if (tex->texture) {
                sdl2r_dispose_texture(tex);
            }
        }
    }
    sdl2r_destroy_hash(ren->th);
    ren->th = 0;
}


static void sdl2r_renderer_free(void *ptr)
{
    struct SDL2RRenderer *ren = ptr;

    if (ren->renderer) {
        sdl2r_dispose_renderer(ren);
    }

    xfree(ren);
}


static void sdl2r_renderer_mark(void *ptr)
{
    struct SDL2RRenderer *ren = ptr;
    rb_gc_mark(ren->vwindow);
    rb_gc_mark(ren->vrender_target_texture);
}


VALUE sdl2r_renderer_alloc(VALUE klass)
{
    struct SDL2RRenderer *ren;
    VALUE vrenderer = TypedData_Make_Struct(klass, struct SDL2RRenderer, &sdl2r_renderer_data_type, ren);
    ren->renderer = 0;
    ren->vwindow = Qnil;
    ren->th = sdl2r_hash_alloc(32);
    return vrenderer;
}


static VALUE sdl2r_destroy_renderer(VALUE klass, VALUE vrenderer)
{
    sdl2r_dispose_renderer(SDL2R_GET_RENDERER_STRUCT(vrenderer));
    return vrenderer;
}


static VALUE sdl2r_renderer_get_destroyed(VALUE self)
{
    struct SDL2RRenderer *ren = SDL2R_GET_STRUCT(Renderer, self);

    return ren->renderer ? Qfalse : Qtrue;
}


static VALUE sdl2r_render_copy(VALUE klass, VALUE vrenderer, VALUE vtexture, VALUE vsrcrect, VALUE vdstrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
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

    if (SDL_RenderCopy(ren->renderer, tex->texture, psr, pdr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_copy_ex(VALUE klass, VALUE vrenderer, VALUE vtexture, VALUE vsrcrect, VALUE vdstrect, VALUE vangle, VALUE vcenter, VALUE vflip)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
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

    if (SDL_RenderCopyEx(ren->renderer, tex->texture, psr, pdr, NUM2DBL(vangle), pp, NUM2INT(vflip))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_present(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);

    SDL_RenderPresent(ren->renderer);

    return vrenderer;
}


static VALUE sdl2r_create_texture_from_surface(VALUE klass, VALUE vrenderer, VALUE vsurface)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    VALUE vtexture = sdl2r_texture_alloc(cTexture);
    struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, vtexture);
    int k;

    SDL2R_RETRY(tex->texture = SDL_CreateTextureFromSurface(ren->renderer, sur->surface));
    if (!tex->texture) {
        rb_raise(eSDLError, SDL_GetError());
    }

    tex->vrenderer = vrenderer;
    k = sdl2r_put_hash(ren->th, (HASHKEY)tex->texture, 0);
    ren->th->vals[k] = vtexture;

    return vtexture;
}


static VALUE sdl2r_create_texture(VALUE klass, VALUE vrenderer, VALUE vformat, VALUE vaccess, VALUE vw, VALUE vh)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    VALUE vtexture = sdl2r_texture_alloc(cTexture);
    struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, vtexture);
    int k;

    SDL2R_RETRY(tex->texture = SDL_CreateTexture(ren->renderer, NUM2UINT(vformat), NUM2INT(vaccess), NUM2INT(vw), NUM2INT(vh)));
    if (!tex->texture) {
        rb_raise(eSDLError, SDL_GetError());
    }

    tex->vrenderer = vrenderer;
    k = sdl2r_put_hash(ren->th, (HASHKEY)tex->texture, 0);
    ren->th->vals[k] = vtexture;

    return vtexture;
}


static VALUE sdl2r_render_clear(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);

    if (SDL_RenderClear(ren->renderer)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_set_render_draw_color(VALUE klass, VALUE vrenderer, VALUE vr, VALUE vg, VALUE vb, VALUE va)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);

    if (SDL_SetRenderDrawColor(ren->renderer, NUM2INT(vr), NUM2INT(vg), NUM2INT(vb), NUM2INT(va))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_get_render_target(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    return ren->vrender_target_texture;
}


static VALUE sdl2r_set_render_target(VALUE klass, VALUE vrenderer, VALUE vtexture)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    SDL_Texture *t;

    if (vtexture == Qnil) {
        t = NULL;
    } else {
        t = SDL2R_GET_TEXTURE_STRUCT(vtexture)->texture;
    }

    if (SDL_SetRenderTarget(ren->renderer, t)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    ren->vrender_target_texture = vtexture;

    return vrenderer;
}


static VALUE sdl2r_set_render_draw_blend_mode(VALUE klass, VALUE vrenderer, VALUE vblendmode)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);

    if (SDL_SetRenderDrawBlendMode(ren->renderer, NUM2INT(vblendmode))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_get_render_draw_blend_mode(VALUE klass, VALUE vrenderer)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    SDL_BlendMode bm;

    if (SDL_GetRenderDrawBlendMode(ren->renderer, &bm)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(bm);
}


static VALUE sdl2r_render_fill_rect(VALUE klass, VALUE vrenderer, VALUE vrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
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

    if (SDL_RenderFillRect(ren->renderer, pr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


//static VALUE sdl2r_renderer_test(VALUE klass, VALUE vrenderer)
//{
//    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
//
//    printf("n_buckets = %d\n", ren->th->n_buckets);
//    printf("size = %d\n", ren->th->size);
//    printf("n_occupied = %d\n", ren->th->n_occupied);
//    return Qnil;
//}


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
    rb_define_singleton_method(mSDL, "get_render_target", sdl2r_get_render_target, 1);
    rb_define_singleton_method(mSDL, "set_render_draw_blend_mode", sdl2r_set_render_draw_blend_mode, 2);
    rb_define_singleton_method(mSDL, "get_render_draw_blend_mode", sdl2r_get_render_draw_blend_mode, 1);
    rb_define_singleton_method(mSDL, "render_fill_rect", sdl2r_render_fill_rect, 2);

//    rb_define_singleton_method(mSDL, "renderer_test", sdl2r_renderer_test, 1);

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

