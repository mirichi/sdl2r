#define SDL2RPIXELFORMAT
#include "sdl2r.h"
#include "sdl2r_hash.h"
#include "sdl2r_pixels.h"
#include "sdl2r_pixelformat.h"
#include "sdl2r_rect.h"

VALUE cPixelFormat;

static void sdl2r_pixelformat_free(void *ptr);
const rb_data_type_t sdl2r_pixelformat_data_type = {
    "PixelFormat",
    {
    0,
    sdl2r_pixelformat_free,
    0,
    },
};


static void sdl2r_pixelformat_free(void *ptr)
{
    struct SDL2RPixelFormat *pxf = ptr;
    if (pxf->format) {
        SDL_FreeFormat(pxf->format);
    }
    xfree(pxf);
}


VALUE sdl2r_pixelformat_alloc(VALUE klass)
{
    struct SDL2RPixelFormat *pxf;
    VALUE vpixelformat = TypedData_Make_Struct(klass, struct SDL2RPixelFormat, &sdl2r_pixelformat_data_type, pxf);
    pxf->format = 0;
    return vpixelformat;
}


static VALUE sdl2r_pixelformat_im_dispose(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);

    SDL_FreeFormat(pxf->format);
    pxf->format = 0;

    return self;
}


static VALUE sdl2r_pixelformat_im_get_disposed(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_STRUCT(PixelFormat, self);
    return SDL2R_TO_BOOL(pxf->format== NULL);
}


static VALUE sdl2r_pixelformat_im_get_format(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return INT2NUM(format->format);
}


static VALUE sdl2r_pixelformat_im_get_bits_per_pixel(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return UINT2NUM(format->BitsPerPixel);
}


static VALUE sdl2r_pixelformat_im_get_bytes_per_pixel(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return UINT2NUM(format->BytesPerPixel);
}


static VALUE sdl2r_pixelformat_im_get_rmask(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return UINT2NUM(format->Rmask);
}


static VALUE sdl2r_pixelformat_im_get_gmask(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return UINT2NUM(format->Gmask);
}


static VALUE sdl2r_pixelformat_im_get_bmask(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return UINT2NUM(format->Bmask);
}


static VALUE sdl2r_pixelformat_im_get_amask(VALUE self)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(self);
    SDL_PixelFormat *format = pxf->format;

    return UINT2NUM(format->Amask);
}


static VALUE sdl2r_alloc_format(VALUE klass, VALUE vpixelformatenum)
{
    VALUE vpixelformat = sdl2r_pixelformat_alloc(cPixelFormat);
    struct SDL2RPixelFormat *pxf = SDL2R_GET_STRUCT(PixelFormat, vpixelformat);
    SDL_PixelFormat *format;

    format = SDL_AllocFormat(NUM2UINT(vpixelformatenum));
    if (!format) {
        rb_raise(eSDLError, "%s", SDL_GetError());
    }
    pxf->format = format;

    return vpixelformat;
}


static VALUE sdl2r_free_format(VALUE klass, VALUE vpixelformat)
{
    struct SDL2RPixelFormat *pxf = SDL2R_GET_PIXELFORMAT_STRUCT(vpixelformat);

    SDL_FreeFormat(pxf->format);
    pxf->format = 0;

    return vpixelformat;
}


void Init_sdl2r_pixelformat(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(alloc_format, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(free_format, 1);

    // SDL::PixelFormat class
    cPixelFormat = rb_define_class_under(mSDL, "PixelFormat", rb_cObject);
    rb_define_alloc_func(cPixelFormat, sdl2r_pixelformat_alloc);
    rb_define_method(cPixelFormat, "dispose", sdl2r_pixelformat_im_dispose, 0);
    rb_define_method(cPixelFormat, "disposed?", sdl2r_pixelformat_im_get_disposed, 0);
    rb_define_method(cPixelFormat, "format", sdl2r_pixelformat_im_get_format, 0);
//    rb_define_method(cPixelFormat, "palette", sdl2r_pixelformat_im_get_palette, 0);
    rb_define_method(cPixelFormat, "bits_per_pixel", sdl2r_pixelformat_im_get_bits_per_pixel, 0);
    rb_define_method(cPixelFormat, "bytes_per_pixel", sdl2r_pixelformat_im_get_bytes_per_pixel, 0);
    rb_define_method(cPixelFormat, "rmask", sdl2r_pixelformat_im_get_rmask, 0);
    rb_define_method(cPixelFormat, "gmask", sdl2r_pixelformat_im_get_gmask, 0);
    rb_define_method(cPixelFormat, "bmask", sdl2r_pixelformat_im_get_bmask, 0);
    rb_define_method(cPixelFormat, "amask", sdl2r_pixelformat_im_get_amask, 0);

    // Constants

    // weak ref hash
}
