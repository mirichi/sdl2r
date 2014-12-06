#define SDL2RRENDERER
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_window.h"
#include "sdl2r_renderer.h"
#include "sdl2r_texture.h"
#include "sdl2r_rect.h"

VALUE cRenderer;

VALUE sdl2r_EnumRendererFlags;

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


void sdl2r_renderer_dispose(struct SDL2RRenderer *ren)
{
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, ren->vwindow);
    SDL_DestroyRenderer(ren->renderer);
    ren->renderer = 0;
    ren->vwindow = Qnil;
    win->vrenderer = Qnil;
    SDL2R_CLEAR_HASH(ren->th, Texture, texture, sdl2r_texture_dispose);
    sdl2r_destroy_hash(ren->th);
    ren->th = 0;
}


static void sdl2r_renderer_free(void *ptr)
{
    struct SDL2RRenderer *ren = ptr;

    if (ren->renderer) {
        sdl2r_renderer_dispose(ren);
    }

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
    VALUE vrenderer = TypedData_Make_Struct(klass, struct SDL2RRenderer, &sdl2r_renderer_data_type, ren);
    ren->renderer = 0;
    ren->vwindow = Qnil;
    ren->th = sdl2r_hash_alloc(32);
    return vrenderer;
}


static VALUE sdl2r_renderer_im_dispose(VALUE self)
{
    sdl2r_renderer_dispose(SDL2R_GET_RENDERER_STRUCT(self));
    return self;
}


static VALUE sdl2r_renderer_im_get_disposed(VALUE self)
{
    struct SDL2RRenderer *ren = SDL2R_GET_STRUCT(Renderer, self);

    return ren->renderer ? Qfalse : Qtrue;
}


static VALUE sdl2r_destroy_renderer(VALUE klass, VALUE vrenderer)
{
    sdl2r_renderer_dispose(SDL2R_GET_RENDERER_STRUCT(vrenderer));
    return vrenderer;
}


static VALUE sdl2r_render_copy(VALUE klass, VALUE vrenderer, VALUE vtexture, VALUE vsrcrect, VALUE vdstrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);
    SDL_Rect srcrect, dstrect;
    SDL_Rect *psr=0, *pdr=0;

    SDL2R_SET_RECT_OR_NULL(psr, srcrect, vsrcrect);
    SDL2R_SET_RECT_OR_NULL(pdr, dstrect, vdstrect);

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

    SDL2R_SET_RECT_OR_NULL(psr, srcrect, vsrcrect);
    SDL2R_SET_RECT_OR_NULL(pdr, dstrect, vdstrect);
    SDL2R_SET_POINT_OR_NULL(pp, point, vcenter);

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

    SDL2R_RETRY(tex->texture = SDL_CreateTextureFromSurface(ren->renderer, sur->surface));
    if (!tex->texture) {
        rb_raise(eSDLError, SDL_GetError());
    }

    tex->vrenderer = vrenderer;
    sdl2r_put_hash(ren->th, (HASHKEY)tex->texture, vtexture);

    return vtexture;
}


static VALUE sdl2r_create_texture(VALUE klass, VALUE vrenderer, VALUE vformat, VALUE vaccess, VALUE vw, VALUE vh)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    VALUE vtexture = sdl2r_texture_alloc(cTexture);
    struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, vtexture);

    SDL2R_RETRY(tex->texture = SDL_CreateTexture(ren->renderer, NUM2UINT(vformat), NUM2INT(vaccess), NUM2INT(vw), NUM2INT(vh)));
    if (!tex->texture) {
        rb_raise(eSDLError, SDL_GetError());
    }

    tex->vrenderer = vrenderer;
    sdl2r_put_hash(ren->th, (HASHKEY)tex->texture, vtexture);

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
    struct SDL2RTexture *tex;
    SDL_Texture *t;
    VALUE vtexture;

    t = SDL_GetRenderTarget(ren->renderer);
    if (!t) {
        return Qnil;
    }

    vtexture = sdl2r_get_hash(ren->th, (HASHKEY)t);
    if (vtexture == Qnil) {
        vtexture = sdl2r_texture_alloc(cTexture);
        tex = SDL2R_GET_STRUCT(Texture, vtexture);
        tex->texture = t;
        tex->vrenderer = vrenderer;
        sdl2r_put_hash(ren->th, (HASHKEY)t, vtexture);
    }
    return vtexture;
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


static VALUE sdl2r_render_draw_point(VALUE klass, VALUE vrenderer, VALUE vx, VALUE vy)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);

    if (SDL_RenderDrawPoint(ren->renderer, NUM2INT(vx), NUM2INT(vy))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_draw_points(VALUE klass, VALUE vrenderer, VALUE vpoints)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    int i;

    Check_Type(vpoints, T_ARRAY);
    {
        int count = RARRAY_LEN(vpoints);
        SDL_Point points[count];
        for (i = 0; i < RARRAY_LEN(vpoints); i++) {
            VALUE vpoint = rb_ary_entry(vpoints, i);
            SDL2R_SET_POINT(points[i], vpoint);
        }

        if (SDL_RenderDrawPoints(ren->renderer, points, count)) {
            rb_raise(eSDLError, SDL_GetError());
        }
    }

    return vrenderer;
}


static VALUE sdl2r_render_draw_line(VALUE klass, VALUE vrenderer, VALUE vx1, VALUE vy1, VALUE vx2, VALUE vy2)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);

    if (SDL_RenderDrawLine(ren->renderer, NUM2INT(vx1), NUM2INT(vy1), NUM2INT(vx2), NUM2INT(vy2))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_draw_lines(VALUE klass, VALUE vrenderer, VALUE vpoints)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    int i;

    Check_Type(vpoints, T_ARRAY);
    {
        int count = RARRAY_LEN(vpoints);
        SDL_Point points[count];
        for (i = 0; i < RARRAY_LEN(vpoints); i++) {
            VALUE vpoint = rb_ary_entry(vpoints, i);
            SDL2R_SET_POINT(points[i], vpoint);
        }

        if (SDL_RenderDrawLines(ren->renderer, points, count)) {
            rb_raise(eSDLError, SDL_GetError());
        }
    }

    return vrenderer;
}


static VALUE sdl2r_render_draw_rect(VALUE klass, VALUE vrenderer, VALUE vrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    SDL_Rect rect;
    SDL_Rect *pr=0;

    SDL2R_SET_RECT_OR_NULL(pr, rect, vrect);

    if (SDL_RenderDrawRect(ren->renderer, pr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_draw_rects(VALUE klass, VALUE vrenderer, VALUE vrects)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    int i;

    Check_Type(vrects, T_ARRAY);
    {
        int count = RARRAY_LEN(vrects);
        SDL_Rect rects[count];
        for (i = 0; i < RARRAY_LEN(vrects); i++) {
            VALUE vrect = rb_ary_entry(vrects, i);
            SDL2R_SET_RECT(rects[i], vrect);
        }

        if (SDL_RenderDrawRects(ren->renderer, rects, count)) {
            rb_raise(eSDLError, SDL_GetError());
        }
    }

    return vrenderer;
}


static VALUE sdl2r_render_fill_rect(VALUE klass, VALUE vrenderer, VALUE vrect)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    SDL_Rect rect;
    SDL_Rect *pr=0;

    SDL2R_SET_RECT_OR_NULL(pr, rect, vrect);

    if (SDL_RenderFillRect(ren->renderer, pr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrenderer;
}


static VALUE sdl2r_render_fill_rects(VALUE klass, VALUE vrenderer, VALUE vrects)
{
    struct SDL2RRenderer *ren = SDL2R_GET_RENDERER_STRUCT(vrenderer);
    int i;

    Check_Type(vrects, T_ARRAY);
    {
        int count = RARRAY_LEN(vrects);
        SDL_Rect rects[count];
        for (i = 0; i < RARRAY_LEN(vrects); i++) {
            VALUE vrect = rb_ary_entry(vrects, i);
            SDL2R_SET_RECT(rects[i], vrect);
        }

        if (SDL_RenderFillRects(ren->renderer, rects, count)) {
            rb_raise(eSDLError, SDL_GetError());
        }
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
    SDL2R_DEFINE_SINGLETON_METHOD(create_texture_from_surface, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(render_copy, 4);
    SDL2R_DEFINE_SINGLETON_METHOD(render_copy_ex, 7);
    SDL2R_DEFINE_SINGLETON_METHOD(render_present, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(render_draw_point, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(render_draw_points, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(render_draw_line, 5);
    SDL2R_DEFINE_SINGLETON_METHOD(render_draw_lines, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(render_draw_rect, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(render_draw_rects, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(render_fill_rect, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(render_fill_rects, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(destroy_renderer, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(render_clear, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_render_draw_color, 5);
    SDL2R_DEFINE_SINGLETON_METHOD(create_texture, 5);
    SDL2R_DEFINE_SINGLETON_METHOD(set_render_target, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(get_render_target, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_render_draw_blend_mode, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(get_render_draw_blend_mode, 1);

//    rb_define_singleton_method(renderer_test", sdl2r_renderer_test, 1);

    // SDL::Renderer class
    cRenderer = rb_define_class_under(mSDL, "Renderer", rb_cObject);
    rb_define_alloc_func(cRenderer, sdl2r_renderer_alloc);

    rb_define_method(cRenderer, "dispose", sdl2r_renderer_im_dispose, 0);
    rb_define_method(cRenderer, "disposed?", sdl2r_renderer_im_get_disposed, 0);

    // enum
    SDL2R_DEFINE_ENUM(EnumRendererFlags);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, RENDERER_SOFTWARE);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, RENDERER_ACCELERATED);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, RENDERER_PRESENTVSYNC);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, RENDERER_TARGETTEXTURE);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, TEXTUREACCESS_STATIC);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, TEXTUREACCESS_STREAMING);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, TEXTUREACCESS_TARGET);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, FLIP_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, FLIP_HORIZONTAL);
    SDL2R_DEFINE_ENUM_VALUE(EnumRendererFlags, FLIP_VERTICAL);
}

