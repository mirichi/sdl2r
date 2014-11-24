#define SDL2RPIXELFORMAT
#include "sdl2r.h"
#include "sdl2r_hash.h"
#include "sdl2r_surface.h"
#include "sdl2r_pixels.h"
#include "sdl2r_pixelformat.h"
#include "sdl2r_rect.h"

VALUE cPixelFormat;

static void sdl2r_pixelformat_mark(void *ptr);
static void sdl2r_pixelformat_free(void *ptr);
const rb_data_type_t sdl2r_pixelformat_data_type = {
    "PixelFormat",
    {
    sdl2r_pixelformat_mark,
    sdl2r_pixelformat_free,
    0,
    },
};


static void sdl2r_pixelformat_mark(void *ptr)
{
    struct SDL2RPixelFormat *pif = ptr;
    rb_gc_mark(pif->vsurface);
}


static void sdl2r_pixelformat_free(void *ptr)
{
    struct SDL2RPixelFormat *pif = ptr;
    if (pif->format) {
        SDL_FreeFormat(pif->format);
    }
    xfree(pif);
}


SDL_PixelFormat *sdl2r_get_sdl_pixelformat(struct SDL2RPixelFormat *pif)
{
    if (pif->vsurface != Qnil) {
        struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(pif->vsurface);
        return sur->surface->format;
    } else if (pif->format) {
        return pif->format;
    } else {
        rb_raise(eSDL2RError, "disposed PixelFormat object");
        return 0;
    }
}


VALUE sdl2r_pixelformat_alloc(VALUE klass)
{
    struct SDL2RPixelFormat *pif;
    VALUE vpixelformat = TypedData_Make_Struct(klass, struct SDL2RPixelFormat, &sdl2r_pixelformat_data_type, pif);
    pif->format = 0;
    pif->vsurface = Qnil;
    return vpixelformat;
}


static VALUE sdl2r_pixelformat_im_dispose(VALUE self)
{
    struct SDL2RPixelFormat *pif = SDL2R_GET_PIXELFORMAT_STRUCT(self);

    if (pif->vsurface != Qnil) {
        rb_raise(eSDL2RError, "Surface's PixelFormat don't free");
    }
    SDL_FreeFormat(pif->format);
    pif->format = 0;

    return self;
}


static VALUE sdl2r_pixelformat_im_get_disposed(VALUE self)
{
    struct SDL2RPixelFormat *pif = SDL2R_GET_STRUCT(PixelFormat, self);
    return SDL2R_TO_BOOL(pif->vsurface == Qnil && pif->format== NULL);
}


static VALUE sdl2r_pixelformat_im_get_format(VALUE self)
{
    struct SDL2RPixelFormat *pif = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = sdl2r_get_sdl_pixelformat(pif);

    return INT2NUM(format->format);
}


static VALUE sdl2r_alloc_format(VALUE klass, VALUE vpixelformatenum)
{
    VALUE vpixelformat = sdl2r_pixelformat_alloc(cPixelFormat);
    struct SDL2RPixelFormat *pif = SDL2R_GET_STRUCT(PixelFormat, vpixelformat);
    SDL_PixelFormat *format;

    format = SDL_AllocFormat(NUM2UINT(vpixelformatenum));
    if (!format) {
        rb_raise(eSDLError, SDL_GetError());
    }
    pif->format = format;

    return vpixelformat;
}


static VALUE sdl2r_free_format(VALUE klass, VALUE vpixelformat)
{
    struct SDL2RPixelFormat *pif = SDL2R_GET_PIXELFORMAT_STRUCT(vpixelformat);

    if (pif->vsurface != Qnil) {
        rb_raise(eSDL2RError, "Surface's PixelFormat don't free");
    }

    SDL_FreeFormat(pif->format);
    pif->format = 0;

    return vpixelformat;
}


void Init_sdl2r_pixelformat(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(alloc_format, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(free_format, 1);

    // SDL::Surface::PixelFormat class
    cPixelFormat = rb_define_class_under(cSurface, "PixelFormat", rb_cObject);
    rb_define_alloc_func(cPixelFormat, sdl2r_pixelformat_alloc);
    rb_define_method(cPixelFormat, "dispose", sdl2r_pixelformat_im_dispose, 0);
    rb_define_method(cPixelFormat, "disposed?", sdl2r_pixelformat_im_get_disposed, 0);
    rb_define_method(cPixelFormat, "format", sdl2r_pixelformat_im_get_format, 0);

    // Constants

    // weak ref hash
}

