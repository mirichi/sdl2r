#define SDL2RSURFACE
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_window.h"
#include "sdl2r_pixels.h"

VALUE cSurface;

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


static VALUE sdl2r_surface_im_dispose(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_SURFACE_STRUCT(self);

    if (sur->vwindow != Qnil) {
        rb_raise(eSDL2RError, "WindowSurface don't free");
    }
    SDL_FreeSurface(sur->surface);
    sur->surface = 0;

    return self;
}


static VALUE sdl2r_surface_im_get_disposed(VALUE self)
{
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, self);
    return sur->surface ? Qfalse : Qtrue;
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


static VALUE sdl2r_macro_MUSTLOCK(VALUE klass, VALUE vsurface)
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


void Init_sdl2r_surface(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(create_rgb_surface, -1);
    SDL2R_DEFINE_SINGLETON_METHOD(free_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(blit_surface, 4);
    SDL2R_DEFINE_SINGLETON_METHOD(lock_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(unlock_surface, 1);

    // SDL macro
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(MUSTLOCK, 1);

    // SDL::Surface class
    cSurface = rb_define_class_under(mSDL, "Surface", rb_cObject);
    rb_define_alloc_func(cSurface, sdl2r_surface_alloc);

    rb_define_method(cSurface, "dispose", sdl2r_surface_im_dispose, 0);
    rb_define_method(cSurface, "disposed?", sdl2r_surface_im_get_disposed, 0);
    rb_define_method(cSurface, "w", sdl2r_w, 0);
    rb_define_method(cSurface, "h", sdl2r_h, 0);
    rb_define_method(cSurface, "pixels", sdl2r_get_pixels, 0);

    // Constants
}

