#define SDL2RIMAGE
#include "sdl2r.h"
#include "sdl2r_image.h"
#include "sdl2r_surface.h"
#include "sdl2r_rwops.h"
#include "sdl2r_version.h"

VALUE mImage;

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


VALUE sdl2r_img_linked_version(VALUE klass)
{
    const SDL_version *v = IMG_Linked_Version();
    VALUE ary[3];

    ary[0] = INT2NUM(v->major);
    ary[1] = INT2NUM(v->minor);
    ary[2] = INT2NUM(v->patch);

    return rb_class_new_instance(3, ary, cVersion);
}


VALUE sdl2r_macro_IMAGE_VERSION(VALUE klass)
{
    SDL_version v;
    VALUE ary[3];

    SDL_IMAGE_VERSION(&v);
    ary[0] = INT2NUM(v.major);
    ary[1] = INT2NUM(v.minor);
    ary[2] = INT2NUM(v.patch);

    return rb_class_new_instance(3, ary, cVersion);
}


void Init_sdl2r_image(void)
{
    mImage = rb_define_module_under(mSDL, "IMG");

#define SDL2R_DEFINE_SINGLETON_METHOD_IMG(name, arity) rb_define_singleton_method(mImage, #name, sdl2r_img_##name, arity)
    SDL2R_DEFINE_SINGLETON_METHOD_IMG(linked_version, 0);

    // Constants
    rb_define_const(mSDL, "IMAGE_VERSION", sdl2r_macro_IMAGE_VERSION(mSDL));
    SDL2R_DEFINE_CONST(IMAGE_MAJOR_VERSION);
    SDL2R_DEFINE_CONST(IMAGE_MINOR_VERSION);
    SDL2R_DEFINE_CONST(IMAGE_PATCHLEVEL);

#define SDL2R_DEFINE_SINGLETON_METHOD_IMAGE(name, arity) rb_define_singleton_method(mImage, #name, sdl2r_img_##name, arity)
    SDL2R_DEFINE_SINGLETON_METHOD_IMAGE(load, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_IMAGE(load_rw, 2);
}


