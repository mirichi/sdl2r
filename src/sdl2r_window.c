#include <SDL.h>

#define SDL2RWINDOW
#include "sdl2r.h"
#include "sdl2r_window.h"
#include "sdl2r_surface.h"
#include "sdl2r_renderer.h"

VALUE cWindow;

static void sdl2r_window_free(void *ptr);

const rb_data_type_t sdl2r_window_data_type = {
    "Window",
    {
    0,
    sdl2r_window_free,
    0,
    },
};

static void sdl2r_window_free(void *ptr)
{
    struct SDL2RWindow *win = ptr;
    if (win->renderer) {
        SDL_DestroyRenderer(win->renderer);
    }
    if (win->window) {
        SDL_DestroyWindow(win->window);
    }
    xfree(win);
}


VALUE sdl2r_window_alloc(VALUE klass)
{
    struct SDL2RWindow *win;
    VALUE vwin = TypedData_Make_Struct(klass, struct SDL2RWindow, &sdl2r_window_data_type, win);
    win->window = 0;
    win->renderer = 0;
    return vwin;
}


static VALUE sdl2r_window_get_destroyed(VALUE self)
{
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, self);
    return win->window ? Qfalse : Qtrue;
}


// SDL module methods
static VALUE sdl2r_get_window_title(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    VALUE vstr;

    vstr = rb_enc_associate(rb_str_new2(SDL_GetWindowTitle(win->window)), g_enc_utf8);

    return vstr;
}


static VALUE sdl2r_set_window_title(VALUE klass, VALUE vwindow, VALUE vstr)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    Check_Type(vstr, T_STRING);

    if (rb_enc_get_index(vstr) != 0) {
        vstr = rb_str_export_to_enc(vstr, g_enc_utf8);
    }
    SDL_SetWindowTitle(win->window, RSTRING_PTR(vstr));

    return vstr;
}


static VALUE sdl2r_get_window_surface(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    VALUE vsurface = sdl2r_surface_alloc(cSurface);
    struct SDL2RSurface *sur = SDL2R_GET_STRUCT(Surface, vsurface);

    sur->surface = SDL_GetWindowSurface(win->window);
    if (!sur->surface) {
        rb_raise(eSDLError, SDL_GetError());
    }
    sur->vwindow = vwindow;

    return vsurface;
}


static VALUE sdl2r_update_window_surface(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    if (SDL_UpdateWindowSurface(win->window)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vwindow;
}


static VALUE sdl2r_create_renderer(VALUE klass, VALUE vwindow, VALUE vindex, VALUE vflags)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    VALUE vrenderer = sdl2r_renderer_alloc(cRenderer);
    struct SDL2RRenderer *ren = SDL2R_GET_STRUCT(Renderer, vrenderer);

    SDL2R_RETRY(win->renderer = SDL_CreateRenderer(win->window, NUM2INT(vindex), (Uint32)NUM2UINT(vflags)));
    if (!win->renderer) {
        rb_raise(eSDLError, SDL_GetError());
    }
    ren->vwindow = vwindow;

    return vrenderer;
}


static VALUE sdl2r_get_renderer(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    VALUE vrenderer = sdl2r_renderer_alloc(cRenderer);
    struct SDL2RRenderer *ren = SDL2R_GET_STRUCT(Renderer, vrenderer);

    win->renderer = SDL_GetRenderer(win->window);
    if (!win->renderer) {
        rb_raise(eSDLError, SDL_GetError());
    }
    ren->vwindow = vwindow;

    return vrenderer;
}


static VALUE sdl2r_create_window(VALUE klass, VALUE vstr, VALUE vx, VALUE vy, VALUE vw, VALUE vh, VALUE vflags)
{
    VALUE vwin = sdl2r_window_alloc(cWindow);
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, vwin);

    SDL2R_RETRY(win->window = SDL_CreateWindow(RSTRING_PTR(vstr), NUM2INT(vx), NUM2INT(vy), NUM2INT(vw), NUM2INT(vh), NUM2INT(vflags)));
    if (!win->window) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vwin;
}


static VALUE sdl2r_destroy_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    if (win->renderer) {
        SDL_DestroyRenderer(win->renderer);
        win->renderer = 0;
    }
    SDL_DestroyWindow(win->window);
    win->window = 0;

    return vwindow;
}


static VALUE sdl2r_get_window_pixel_format(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    Uint32 result;

    if ((result = SDL_GetWindowPixelFormat(win->window)) == SDL_PIXELFORMAT_UNKNOWN) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return UINT2NUM(result);
}


static VALUE sdl2r_show_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_ShowWindow(win->window);

    return vwindow;
}


static VALUE sdl2r_set_window_size(VALUE klass, VALUE vwindow, VALUE vw, VALUE vh)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_SetWindowSize(win->window, NUM2INT(vw), NUM2INT(vh));

    return vwindow;
}


void Init_sdl2r_window(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "create_window", sdl2r_create_window, 6);
    rb_define_singleton_method(mSDL, "destroy_window", sdl2r_destroy_window, 1);
    rb_define_singleton_method(mSDL, "create_renderer", sdl2r_create_renderer, 3);
    rb_define_singleton_method(mSDL, "get_renderer", sdl2r_get_renderer, 1);
    rb_define_singleton_method(mSDL, "get_window_surface", sdl2r_get_window_surface, 1);
    rb_define_singleton_method(mSDL, "update_window_surface", sdl2r_update_window_surface, 1);
    rb_define_singleton_method(mSDL, "get_window_title", sdl2r_get_window_title, 1);
    rb_define_singleton_method(mSDL, "set_window_title", sdl2r_set_window_title, 2);
    rb_define_singleton_method(mSDL, "get_window_pixel_format", sdl2r_get_window_pixel_format, 1);
    rb_define_singleton_method(mSDL, "show_window", sdl2r_show_window, 1);
    rb_define_singleton_method(mSDL, "set_window_size", sdl2r_set_window_size, 3);

    // SDL::Window class
    cWindow = rb_define_class_under(mSDL, "Window", rb_cObject);
    rb_define_alloc_func(cWindow, sdl2r_window_alloc);

    rb_define_method(cWindow, "destroyed?", sdl2r_window_get_destroyed, 0);

    // Constants
    rb_define_const(mSDL, "WINDOWPOS_CENTERED", INT2FIX(SDL_WINDOWPOS_CENTERED));
    rb_define_const(mSDL, "WINDOWPOS_UNDEFINED", INT2FIX(SDL_WINDOWPOS_UNDEFINED));

    rb_define_const(mSDL, "WINDOW_FULLSCREEN", INT2FIX(SDL_WINDOW_FULLSCREEN));
    rb_define_const(mSDL, "WINDOW_FULLSCREEN_DESKTOP", INT2FIX(SDL_WINDOW_FULLSCREEN_DESKTOP));
    rb_define_const(mSDL, "WINDOW_OPENGL", INT2FIX(SDL_WINDOW_OPENGL));
    rb_define_const(mSDL, "WINDOW_SHOWN", INT2FIX(SDL_WINDOW_SHOWN));
    rb_define_const(mSDL, "WINDOW_HIDDEN", INT2FIX(SDL_WINDOW_HIDDEN));
    rb_define_const(mSDL, "WINDOW_BORDERLESS", INT2FIX(SDL_WINDOW_BORDERLESS));
    rb_define_const(mSDL, "WINDOW_RESIZABLE", INT2FIX(SDL_WINDOW_RESIZABLE));
    rb_define_const(mSDL, "WINDOW_MINIMIZED", INT2FIX(SDL_WINDOW_MINIMIZED));
    rb_define_const(mSDL, "WINDOW_MAXIMIZED", INT2FIX(SDL_WINDOW_MAXIMIZED));
    rb_define_const(mSDL, "WINDOW_INPUT_GRABBED", INT2FIX(SDL_WINDOW_INPUT_GRABBED));
    rb_define_const(mSDL, "WINDOW_INPUT_FOCUS", INT2FIX(SDL_WINDOW_INPUT_FOCUS));
    rb_define_const(mSDL, "WINDOW_MOUSE_FOCUS", INT2FIX(SDL_WINDOW_MOUSE_FOCUS));
    rb_define_const(mSDL, "WINDOW_FOREIGN", INT2FIX(SDL_WINDOW_FOREIGN));
    rb_define_const(mSDL, "WINDOW_ALLOW_HIGHDPI", INT2FIX(SDL_WINDOW_ALLOW_HIGHDPI));
}
