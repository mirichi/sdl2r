#define SDL2RWINDOW
#include "sdl2r.h"
#include "sdl2r_window.h"
#include "sdl2r_surface.h"
#include "sdl2r_renderer.h"
#include "sdl2r_hash.h"

VALUE cWindow;
static struct SDL2RHash *wh;

static void sdl2r_window_free(void *ptr);

const rb_data_type_t sdl2r_window_data_type = {
    "Window",
    {
    0,
    sdl2r_window_free,
    0,
    },
};


void sdl2r_dispose_window(struct SDL2RWindow *win)
{
    if (win->vrenderer != Qnil) {
        sdl2r_dispose_renderer(SDL2R_GET_STRUCT(Renderer, win->vrenderer));
    }

    SDL_DestroyWindow(win->window);
    sdl2r_del_hash(wh, sdl2r_get_hash(wh, (HASHKEY)win->window));
    win->window = 0;
}


static void sdl2r_window_free(void *ptr)
{
    struct SDL2RWindow *win = ptr;

    if (win->window) {
        sdl2r_dispose_window(win);
    }

    xfree(win);
}


VALUE sdl2r_window_alloc(VALUE klass)
{
    struct SDL2RWindow *win;

    VALUE vwindow = TypedData_Make_Struct(klass, struct SDL2RWindow, &sdl2r_window_data_type, win);
    win->window = 0;
    win->vrenderer = Qnil;

    return vwindow;
}


static VALUE sdl2r_destroy_window(VALUE klass, VALUE vwindow)
{
    sdl2r_dispose_window(SDL2R_GET_WINDOW_STRUCT(vwindow));
    return vwindow;
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

    SDL2R_RETRY(ren->renderer = SDL_CreateRenderer(win->window, NUM2INT(vindex), (Uint32)NUM2UINT(vflags)));
    if (!ren->renderer) {
        rb_raise(eSDLError, SDL_GetError());
    }
    ren->vwindow = vwindow;
    win->vrenderer = vrenderer;

    return vrenderer;
}


static VALUE sdl2r_get_renderer(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    return win->vrenderer;
}


static VALUE sdl2r_create_window(VALUE klass, VALUE vstr, VALUE vx, VALUE vy, VALUE vw, VALUE vh, VALUE vflags)
{
    VALUE vwindow = sdl2r_window_alloc(cWindow);
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, vwindow);
    int k;

    SDL2R_RETRY(win->window = SDL_CreateWindow(RSTRING_PTR(vstr), NUM2INT(vx), NUM2INT(vy), NUM2INT(vw), NUM2INT(vh), NUM2INT(vflags)));
    if (!win->window) {
        rb_raise(eSDLError, SDL_GetError());
    }
    k = sdl2r_put_hash(wh, (HASHKEY)win->window, 0);
    wh->vals[k] = vwindow;

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


static VALUE sdl2r_get_window_id(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    return UINT2NUM(SDL_GetWindowID(win->window));
}


static VALUE sdl2r_get_window_from_id(VALUE klass, VALUE vid)
{
    VALUE vwindow;
    SDL_Window *w;
    int k;

    w = SDL_GetWindowFromID(NUM2UINT(vid));
    if (!w) {
        rb_raise(eSDLError, SDL_GetError());
    }

    k = sdl2r_get_hash(wh, (HASHKEY)w);
    vwindow = wh->vals[k];

    return vwindow;
}


//static VALUE sdl2r_window_test(VALUE klass)
//{
//    printf("n_buckets = %d\n", wh->n_buckets);
//    printf("size = %d\n", wh->size);
//    printf("n_occupied = %d\n", wh->n_occupied);
//    return Qnil;
//}


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

    rb_define_singleton_method(mSDL, "get_window_id", sdl2r_get_window_id, 1);
    rb_define_singleton_method(mSDL, "get_window_from_id", sdl2r_get_window_from_id, 1);

//    rb_define_singleton_method(mSDL, "window_test", sdl2r_window_test, 0);

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

    wh = sdl2r_hash_alloc(8);
}
