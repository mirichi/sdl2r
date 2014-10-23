#define SDL2RIMAGE
#include "sdl2r.h"
#include "sdl2r_image.h"
#include "sdl2r_surface.h"
#include "sdl2r_rwops.h"


static VALUE sdl2r_img_load(VALUE klass, VALUE vfilename)
{
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    SDL2R_RETRY(sur->surface = IMG_Load(RSTRING_PTR(vfilename)));
    if (!sur->surface) {
        rb_raise(eSDLError, IMG_GetError());
    }
    return vsurface;
}


static VALUE sdl2r_img_load_rw(VALUE klass, VALUE vrwops, VALUE vfreesrc)
{
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    SDL2R_RETRY(sur->surface = IMG_Load_RW(rw->rwops, NUM2INT(vfreesrc)));
    if (!sur->surface) {
        rb_raise(eSDLError, IMG_GetError());
    }

    if (NUM2INT(vfreesrc) != 0) {
        rw->rwops = 0;
        rw->vstr = Qnil;
    }

    return vsurface;
}


void Init_sdl2r_image(void)
{
    mImage = rb_define_module_under(mSDL, "IMG");
    rb_define_singleton_method(mImage, "load", sdl2r_img_load, 1);
    rb_define_singleton_method(mImage, "load_rw", sdl2r_img_load_rw, 2);
}


