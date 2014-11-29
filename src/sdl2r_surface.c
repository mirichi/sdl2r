#define SDL2RSURFACE
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_window.h"
#include "sdl2r_pixels.h"
#include "sdl2r_pixelformat.h"
#include "sdl2r_rect.h"

VALUE cSurface;

static VALUE sdl2r_EnumPixelFormat;
static VALUE sdl2r_EnumPixelType;
static VALUE sdl2r_EnumBitmapOrder;
static VALUE sdl2r_EnumBlendMode;
static VALUE sdl2r_EnumPackOrder;
static VALUE sdl2r_EnumArrayOrder;
static VALUE sdl2r_EnumPackedLayout;

static void sdl2r_surface_free(void *ptr);
static void sdl2r_surface_mark(void *ptr);
const rb_data_type_t sdl2r_surface_data_type = {
    "Surface",
    {
    sdl2r_surface_mark,
    sdl2r_surface_free,
    0,
    },
};


static void sdl2r_surface_mark(void *ptr)
{
    struct SDL2RSurface *sur = ptr;
    rb_gc_mark(sur->vwindow);
    rb_gc_mark(sur->vpixels);
    rb_gc_mark(sur->vpixelformat);
}


static void sdl2r_surface_free(void *ptr)
{
    struct SDL2RSurface *sur = ptr;
    if (sur->surface) {
        SDL_FreeSurface(sur->surface);
    }
    xfree(sur);
}


SDL_Surface *sdl2r_get_sdl_surface(struct SDL2RSurface *sur)
{
    if (sur->vwindow != Qnil) {
        struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(sur->vwindow);
        return SDL_GetWindowSurface(win->window);
    } else if (sur->surface) {
        return sur->surface;
    } else {
        rb_raise(eSDL2RError, "disposed Surface object");
        return 0;
    }
}


VALUE sdl2r_surface_alloc(VALUE klass)
{
    struct SDL2RSurface *sur;
    VALUE vsurface = TypedData_Make_Struct(klass, struct SDL2RSurface, &sdl2r_surface_data_type, sur);
    sur->surface = 0;
    sur->vwindow = Qnil;
    sur->vpixels = Qnil;
    sur->vpixelformat = Qnil;
    return vsurface;
}


static VALUE sdl2r_surface_im_dispose(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);

    if (sur->vwindow != Qnil) {
        rb_raise(eSDL2RError, "WindowSurface don't free");
    }
    SDL_FreeSurface(sur->surface);
    sur->surface = 0;
    sur->vpixels = Qnil;
    sur->vpixelformat = Qnil;

    return self;
}


static VALUE sdl2r_surface_im_get_disposed(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, self);
    return SDL2R_TO_BOOL(sur->surface == NULL && sur->vwindow == Qnil);
}


static VALUE sdl2r_free_surface(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);

    if (sur->vwindow != Qnil) {
        rb_raise(eSDL2RError, "WindowSurface don't free");
    }
    SDL_FreeSurface(sur->surface);
    sur->surface = 0;
    sur->vpixels = Qnil;
    sur->vpixelformat = Qnil;

    return vsurface;
}


static VALUE sdl2r_im_get_w(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);
    return INT2NUM(sdl2r_get_sdl_surface(sur)->w);
}


static VALUE sdl2r_im_get_h(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);
    return INT2NUM(sdl2r_get_sdl_surface(sur)->h);
}


static VALUE sdl2r_im_get_pixels(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);

    if (sur->vpixels != Qnil) {
        return sur->vpixels;
    } else {
        VALUE vpixels = sdl2r_pixels_alloc(cPixels);
        struct SDL2RPixels *pix = SDL2R_GET_STRUCT(Pixels, vpixels);
        pix->vsurface = self;
        sur->vpixels = vpixels;
        return vpixels;
    }
}


static VALUE sdl2r_im_get_format(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);

    if (sur->vpixelformat != Qnil && SDL2R_GET_STRUCT(PixelFormat, sur->vpixelformat)->format != NULL) {
        return sur->vpixelformat;
    } else {
        VALUE vpixelformat = sdl2r_pixelformat_alloc(cPixelFormat);
        struct SDL2RPixelFormat *pxf = SDL2R_GET_STRUCT(PixelFormat, vpixelformat);
        pxf->format = sur->surface->format;
        pxf->format->refcount++;
        sur->vpixelformat = vpixelformat;
        return vpixelformat;
    }
}


static VALUE sdl2r_blit_surface(VALUE klass, VALUE vsrc, VALUE vsrcrect, VALUE vdst, VALUE vdstrect)
{
    struct SDL2RSurface *src = SDL2R_GET_SURFACE_STRUCT(vsrc);
    struct SDL2RSurface *dst = SDL2R_GET_SURFACE_STRUCT(vdst);
    SDL_Rect srcrect, dstrect;
    SDL_Rect *psr=0, *pdr=0;

    SDL2R_SET_RECT_OR_NULL(psr, srcrect, vsrcrect);
    SDL2R_SET_RECT_OR_NULL(pdr, dstrect, vdstrect);

    if (SDL_BlitSurface(sdl2r_get_sdl_surface(src), psr, sdl2r_get_sdl_surface(dst), pdr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return klass;
}


static VALUE sdl2r_blit_scaled(VALUE klass, VALUE vsrc, VALUE vsrcrect, VALUE vdst, VALUE vdstrect)
{
    struct SDL2RSurface *src = SDL2R_GET_SURFACE_STRUCT(vsrc);
    struct SDL2RSurface *dst = SDL2R_GET_SURFACE_STRUCT(vdst);
    SDL_Rect srcrect, dstrect;
    SDL_Rect *psr=0, *pdr=0;

    SDL2R_SET_RECT_OR_NULL(psr, srcrect, vsrcrect);
    SDL2R_SET_RECT_OR_NULL(pdr, dstrect, vdstrect);

    if (SDL_BlitScaled(sdl2r_get_sdl_surface(src), psr, sdl2r_get_sdl_surface(dst), pdr)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return klass;
}


static VALUE sdl2r_create_rgb_surface(int argc, VALUE *argv, VALUE klass)
{
    VALUE vflags, vw, vh, vdepth, vrmask, vgmask, vbmask, vamask;
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    rb_scan_args(argc, argv, "35", &vflags, &vw, &vh, &vdepth, &vrmask, &vgmask, &vbmask, &vamask);

    if (vdepth == Qnil) vdepth = INT2FIX(32);
    if (vrmask == Qnil) vrmask = INT2FIX(0);
    if (vgmask == Qnil) vgmask = INT2FIX(0);
    if (vbmask == Qnil) vbmask = INT2FIX(0);
    if (vamask == Qnil) vamask = INT2FIX(0);

    SDL2R_RETRY(sur->surface = SDL_CreateRGBSurface(NUM2INT(vflags), NUM2INT(vw), NUM2INT(vh), NUM2INT(vdepth), NUM2UINT(vrmask), NUM2UINT(vgmask), NUM2UINT(vbmask), NUM2UINT(vamask)));

    if (!sur->surface) {
        rb_raise(eSDLError, SDL_GetError());
    }
    return vsurface;
}


static VALUE sdl2r_macro_MUSTLOCK(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    return SDL2R_TO_BOOL(SDL_MUSTLOCK(sdl2r_get_sdl_surface(sur)));
}


static VALUE sdl2r_lock_surface(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);

    if (SDL_LockSurface(sdl2r_get_sdl_surface(sur))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vsurface;
}


static VALUE sdl2r_unlock_surface(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);

    SDL_UnlockSurface(sdl2r_get_sdl_surface(sur));

    return vsurface;
}


static VALUE sdl2r_fill_rect(VALUE klass, VALUE vsurface, VALUE vrect, VALUE vcolor)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    SDL_PixelFormat *format = surface->format;
    SDL_Rect rect, *prect;
    SDL_Color col;

    SDL2R_SET_RECT_OR_NULL(prect, rect, vrect);
    SDL2R_SET_COLOR(col, vcolor);

    if (SDL_FillRect(surface, prect, SDL_MapRGBA(format, col.r, col.g, col.b, col.a))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vsurface;
}


static VALUE sdl2r_fill_rects(VALUE klass, VALUE vsurface, VALUE vrects, VALUE vcolor)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    SDL_PixelFormat *format = surface->format;
    SDL_Color col;

    Check_Type(vrects, T_ARRAY);
    SDL2R_SET_COLOR(col, vcolor);
    {
        SDL_Rect rects[RARRAY_LEN(vrects)];
        int i;

        for (i = 0; i < RARRAY_LEN(vrects); i++) {
            SDL2R_SET_RECT(rects[i], rb_ary_entry(vrects, i));
        }

        if (SDL_FillRects(surface, rects, RARRAY_LEN(vrects), SDL_MapRGBA(format, col.r, col.g, col.b, col.a))) {
            rb_raise(eSDLError, SDL_GetError());
        }
    }

    return vsurface;
}


static VALUE sdl2r_get_surface_alpha_mod(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    Uint8 alpha;

    if (SDL_GetSurfaceAlphaMod(surface, &alpha)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(alpha);
}


static VALUE sdl2r_get_surface_blend_mode(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    SDL_BlendMode blendmode;

    if (SDL_GetSurfaceBlendMode(surface, &blendmode)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(blendmode);
}


static VALUE sdl2r_get_surface_color_mod(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    Uint8 r, g, b;

    if (SDL_GetSurfaceColorMod(surface, &r, &g, &b)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return rb_ary_new3(3, INT2NUM(r), INT2NUM(g), INT2NUM(b));
}


static VALUE sdl2r_set_surface_alpha_mod(VALUE klass, VALUE vsurface, VALUE valpha)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);

    if (SDL_SetSurfaceAlphaMod(surface, NUM2INT(valpha))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return Qnil;
}


static VALUE sdl2r_set_surface_blend_mode(VALUE klass, VALUE vsurface, VALUE vblendmode)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);

    if (SDL_SetSurfaceBlendMode(surface, NUM2INT(vblendmode))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return Qnil;
}


static VALUE sdl2r_set_surface_color_mod(VALUE klass, VALUE vsurface, VALUE vr, VALUE vg, VALUE vb)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);

    if (SDL_SetSurfaceColorMod(surface, NUM2INT(vr), NUM2INT(vg), NUM2INT(vb))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return Qnil;
}


static VALUE sdl2r_get_clip_rect(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    SDL_Rect rect;

    SDL_GetClipRect(surface, &rect);

    return sdl2r_rect2vrect(&rect);
}


static VALUE sdl2r_set_clip_rect(VALUE klass, VALUE vsurface, VALUE vrect)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    SDL_Surface *surface = sdl2r_get_sdl_surface(sur);
    SDL_Rect rect, *prect;
    SDL_bool result;

    SDL2R_SET_RECT_OR_NULL(prect, rect, vrect);

    result = SDL_SetClipRect(surface, prect);

    return SDL2R_TO_BOOL(result);
}


void Init_sdl2r_surface(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(create_rgb_surface, -1);
    SDL2R_DEFINE_SINGLETON_METHOD(free_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(blit_scaled, 4);
    SDL2R_DEFINE_SINGLETON_METHOD(blit_surface, 4);
    SDL2R_DEFINE_SINGLETON_METHOD(lock_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(unlock_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(fill_rect, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(fill_rects, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(get_surface_alpha_mod, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_surface_blend_mode, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_surface_color_mod, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_surface_alpha_mod, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(set_surface_blend_mode, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(set_surface_color_mod, 4);
    SDL2R_DEFINE_SINGLETON_METHOD(get_clip_rect, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_clip_rect, 2);

    // SDL macro
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(MUSTLOCK, 1);

    // SDL::Surface class
    cSurface = rb_define_class_under(mSDL, "Surface", rb_cObject);
    rb_define_alloc_func(cSurface, sdl2r_surface_alloc);

    rb_define_method(cSurface, "dispose", sdl2r_surface_im_dispose, 0);
    rb_define_method(cSurface, "disposed?", sdl2r_surface_im_get_disposed, 0);
    rb_define_method(cSurface, "w", sdl2r_im_get_w, 0);
    rb_define_method(cSurface, "h", sdl2r_im_get_h, 0);
    rb_define_method(cSurface, "pixels", sdl2r_im_get_pixels, 0);
    rb_define_method(cSurface, "format", sdl2r_im_get_format, 0);

    // define enum
    SDL2R_DEFINE_ENUM(EnumPixelFormat);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX1LSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX1MSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX4LSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX4MSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB332);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBA4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ABGR4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRA4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB1555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBA5551);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ABGR1555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRA5551);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB565);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR565);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB24);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR24);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBX8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRX8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBA8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ABGR8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRA8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB2101010);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_YV12);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_IYUV);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_YUY2);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_UYVY);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_YVYU);

    SDL2R_DEFINE_ENUM(EnumPixelType);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_INDEX1);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_INDEX4);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_INDEX8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_PACKED8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_PACKED16);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_PACKED32);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYU8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYU16);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYU32);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYF16);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYF32);

    SDL2R_DEFINE_ENUM(EnumBitmapOrder);
    SDL2R_DEFINE_ENUM_VALUE(EnumBitmapOrder, BITMAPORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumBitmapOrder, BITMAPORDER_4321);
    SDL2R_DEFINE_ENUM_VALUE(EnumBitmapOrder, BITMAPORDER_1234);

    SDL2R_DEFINE_ENUM(EnumPackOrder);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_XRGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_RGBX);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_ARGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_RGBA);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_XBGR);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_BGRX);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_ABGR);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_BGRA);

    SDL2R_DEFINE_ENUM(EnumArrayOrder);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_RGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_RGBA);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_ARGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_BGR);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_BGRA);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_ABGR);

    SDL2R_DEFINE_ENUM(EnumPackedLayout);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_332);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_1555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_5551);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_565);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_2101010);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_1010102);

    SDL2R_DEFINE_ENUM(EnumBlendMode);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_BLEND);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_ADD);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_MOD);

    // Constants
}

