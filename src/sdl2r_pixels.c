#define SDL2RPIXELS
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_pixels.h"

VALUE cPixels;

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


void Init_sdl2r_pixels(void)
{
    // SDL::Surface::Pixels class
    cPixels = rb_define_class_under(cSurface, "Pixels", rb_cObject);
    rb_define_alloc_func(cPixels, sdl2r_pixels_alloc);
    rb_define_method(cPixels, "[]", sdl2r_get_pixel, 2);
    rb_define_method(cPixels, "[]=", sdl2r_set_pixel, 3);


    // Constants
}

