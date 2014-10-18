#include <SDL.h>
#include <SDL_image.h>

#define SDL2RIMAGE
#include "sdl2r.h"
#include "sdl2r_image.h"
#include "sdl2r_surface.h"


static VALUE sdl2r_img_load(VALUE klass, VALUE vfilename)
{
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    sur->surface = IMG_Load(RSTRING_PTR(vfilename));
    if (!sur->surface) {
        rb_raise(eSDLError, IMG_GetError());
    }
    return vsurface;
}


void Init_sdl2r_image(void)
{
    mImage = rb_define_module_under(mSDL, "IMG");
    rb_define_singleton_method(mImage, "load", sdl2r_img_load, 1);
}


