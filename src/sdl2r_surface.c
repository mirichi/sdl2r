#include <SDL.h>

#define SDL2RSURFACE
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_window.h"

VALUE cSurface;
VALUE cPixels;

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
static void sdl2r_pixels_free(void *ptr);
static void sdl2r_pixels_mark(void *ptr);
const rb_data_type_t sdl2r_pixels_data_type = {
    "Pixels",
    {
    sdl2r_pixels_mark,
    sdl2r_pixels_free,
    0,
    },
};


// WindowSurfaceはWindowが持つ特別なSurfaceで、自分でDestroyしなくてもよい(したらだめ)。
// それ以外は自分でDestroyする。

static void sdl2r_surface_free(void *ptr)
{
    struct SDL2RSurface *sur = ptr;
    if (sur->surface && sur->vwindow == Qnil) {
        SDL_FreeSurface(sur->surface);
    }
    xfree(sur);
}


static void sdl2r_surface_mark(void *ptr)
{
    struct SDL2RSurface *sur = ptr;
    rb_gc_mark(sur->vwindow);
}


struct SDL2RSurface *sdl2r_get_surface(VALUE vsur)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsur);

    if (sur->vwindow != Qnil) {
        struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(sur->vwindow);
        sur->surface = SDL_GetWindowSurface(win->window);
    }

    return sur;
}


VALUE sdl2r_surface_alloc(VALUE klass)
{
    struct SDL2RSurface *sur;
    VALUE vsurface = TypedData_Make_Struct(klass, struct SDL2RSurface, &sdl2r_surface_data_type, sur);
    sur->surface = 0;
    sur->vwindow = Qnil;
    return vsurface;
}


static VALUE sdl2r_surface_get_destroyed(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, self);
    return sur->surface ? Qfalse : Qtrue;
}


static VALUE sdl2r_w(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);
    return INT2NUM(sur->surface->w);
}


static VALUE sdl2r_h(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);
    return INT2NUM(sur->surface->h);
}


static VALUE sdl2r_get_pixels(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);
    VALUE vpixels = sdl2r_pixels_alloc(cPixels);
    struct SDL2RPixels *pix = SDL2R_GET_STRUCT(Pixels, vpixels);
    (void)sur;
    pix->vsurface = self;

    return vpixels;
}


static VALUE sdl2r_blit_surface(VALUE klass, VALUE vsrc, VALUE vsrcrect, VALUE vdst, VALUE vdstrect)
{
    struct SDL2RSurface *src = sdl2r_get_surface(vsrc);
    struct SDL2RSurface *dst = sdl2r_get_surface(vdst);
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
        dstrect.w = dstrect.h = 0;
        pdr = &dstrect;
    }

    if (SDL_BlitSurface(src->surface, psr, dst->surface, pdr)) {
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


static VALUE sdl2r_mustlock(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);
    return SDL_MUSTLOCK(sur->surface) ? Qtrue : Qfalse;
}


static VALUE sdl2r_lock_surface(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);

    if (SDL_LockSurface(sur->surface)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vsurface;
}


static VALUE sdl2r_unlock_surface(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);

    SDL_UnlockSurface(sur->surface);

    return vsurface;
}


static VALUE sdl2r_free_surface(VALUE klass, VALUE vsurface)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(vsurface);

    if (sur->vwindow != Qnil) {
        rb_raise(eSDL2RError, "WindowSurface don't free");
    }
    SDL_FreeSurface(sur->surface);
    sur->surface = 0;

    return vsurface;
}


// SDL::Surface::Pixels class
static void sdl2r_pixels_free(void *ptr)
{
    struct SDL2RPixels *pix = ptr;
    xfree(pix);
}


static void sdl2r_pixels_mark(void *ptr)
{
    struct SDL2RPixels *pix = ptr;
    rb_gc_mark(pix->vsurface);
}


VALUE sdl2r_pixels_alloc(VALUE klass)
{
    struct SDL2RPixels *pix;
    VALUE vpixels = TypedData_Make_Struct(klass, struct SDL2RPixels, &sdl2r_pixels_data_type, pix);
    pix->vsurface = Qnil;
    return vpixels;
}


static VALUE sdl2r_get_pixel(VALUE self, VALUE vx, VALUE vy)
{
    struct SDL2RPixels *pix = SDL2R_GET_PIXELS_STRUCT(self);
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(pix->vsurface);
    SDL_Surface *surface = sur->surface;
    SDL_PixelFormat *format = surface->format;
    Uint32 pixel;
    Uint8 r, g, b, a;
    VALUE ary[4];
    int x = NUM2INT(vx);
    int y = NUM2INT(vy);

    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        rb_raise(eSDL2RError, "out of range");
    }

    switch(format->BytesPerPixel) {
    case 1:
        pixel = *((Uint8 *)surface->pixels + x + y * surface->w);
        break;
    case 2:
        pixel = *((Uint16 *)surface->pixels + x + y * surface->w);
        break;
    case 3:
        pixel = *((Uint32 *)((Uint8 *)surface->pixels + x * 3 + y * surface->w * 3));
        break;
    case 4:
        pixel = *((Uint32 *)surface->pixels + x + y * surface->w);
        break;
    default:
        rb_raise(eSDL2RError, "internal error");
    }

    SDL_GetRGBA(pixel, format, &r, &g, &b, &a);

    ary[0] = INT2FIX(r);
    ary[1] = INT2FIX(g);
    ary[2] = INT2FIX(b);
    ary[3] = INT2FIX(a);
    return rb_class_new_instance(4, ary, cColor);
}


static VALUE sdl2r_set_pixel(VALUE self, VALUE vx, VALUE vy, VALUE vcolor)
{
    struct SDL2RPixels *pix = SDL2R_GET_PIXELS_STRUCT(self);
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(pix->vsurface);
    SDL_Surface *surface = sur->surface;
    SDL_PixelFormat *format = surface->format;
    Uint32 pixel;
    int x = NUM2INT(vx);
    int y = NUM2INT(vy);

    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        rb_raise(eSDL2RError, "out of range");
    }

    Check_Type(vcolor, T_ARRAY);
    pixel = SDL_MapRGBA(format, (Uint8)NUM2INT(rb_ary_entry(vcolor, 0))
                              , (Uint8)NUM2INT(rb_ary_entry(vcolor, 1))
                              , (Uint8)NUM2INT(rb_ary_entry(vcolor, 2))
                              , (Uint8)NUM2INT(rb_ary_entry(vcolor, 3)));
    switch(format->BytesPerPixel) {
    case 1:
        *((Uint8 *)surface->pixels + x +  y * surface->w) = (Uint8)pixel;
        break;
    case 2:
        *((Uint16 *)surface->pixels + x +  y * surface->w) = (Uint16)pixel;
        break;
    case 3:
        *((Uint8 *)surface->pixels + x * 3 + y * surface->w * 3) = (Uint8)(pixel & 0x000000ff);
        *((Uint8 *)surface->pixels + x * 3 + y * surface->w * 3 + 1) = (Uint8)((pixel & 0x0000ff00)>>8);
        *((Uint8 *)surface->pixels + x * 3 + y * surface->w * 3 + 2) = (Uint8)((pixel & 0x00ff0000)>>16);
        break;
    case 4:
        *((Uint32 *)surface->pixels + x + y * surface->w) = pixel;
        break;
    default:
        rb_raise(eSDL2RError, "internal error");
    }

    return self;
}


void Init_sdl2r_surface(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "create_rgb_surface", sdl2r_create_rgb_surface, -1);
    rb_define_singleton_method(mSDL, "free_surface", sdl2r_free_surface, 1);
    rb_define_singleton_method(mSDL, "blit_surface", sdl2r_blit_surface, 4);
    rb_define_singleton_method(mSDL, "MUSTLOCK", sdl2r_mustlock, 1);
    rb_define_singleton_method(mSDL, "lock_surface", sdl2r_lock_surface, 1);
    rb_define_singleton_method(mSDL, "unlock_surface", sdl2r_unlock_surface, 1);

    // SDL::Surface class
    cSurface = rb_define_class_under(mSDL, "Surface", rb_cObject);
    rb_define_alloc_func(cSurface, sdl2r_surface_alloc);

    rb_define_method(cSurface, "destroyed?", sdl2r_surface_get_destroyed, 0);
    rb_define_method(cSurface, "w", sdl2r_w, 0);
    rb_define_method(cSurface, "h", sdl2r_h, 0);
    rb_define_method(cSurface, "pixels", sdl2r_get_pixels, 0);

    // SDL::Surface::Pixels class
    cPixels = rb_define_class_under(cSurface, "Pixels", rb_cObject);
    rb_define_alloc_func(cPixels, sdl2r_pixels_alloc);
    rb_define_method(cPixels, "[]", sdl2r_get_pixel, 2);
    rb_define_method(cPixels, "[]=", sdl2r_set_pixel, 3);


    // Constants
//    rb_define_const(mSDL, "WINDOWPOS_CENTERED", INT2FIX(SDL_WINDOWPOS_CENTERED));
//    rb_define_const(mSDL, "WINDOWPOS_UNDEFINED", INT2FIX(SDL_WINDOWPOS_UNDEFINED));
}

