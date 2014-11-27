#define SDL2RWINDOW
#include "sdl2r.h"
#include "sdl2r_window.h"
#include "sdl2r_surface.h"
#include "sdl2r_renderer.h"
#include "sdl2r_opengl.h"
#include "sdl2r_hash.h"

VALUE cWindow;
struct SDL2RHash *sdl2r_window_hash;

static void sdl2r_window_free(void *ptr);

const rb_data_type_t sdl2r_window_data_type = {
    "Window",
    {
    0,
    sdl2r_window_free,
    0,
    },
};


void sdl2r_window_dispose(struct SDL2RWindow *win)
{
    if (win->vrenderer != Qnil) {
        sdl2r_renderer_dispose(SDL2R_GET_STRUCT(Renderer, win->vrenderer));
    }

    if (win->vglcontext != Qnil) {
        sdl2r_glcontext_dispose(SDL2R_GET_STRUCT(GLContext, win->vglcontext));
    }

    if (win->vwindowsurface != Qnil) {
        SDL2R_GET_STRUCT(Surface, win->vwindowsurface)->vwindow = Qnil;
        win->vwindowsurface = Qnil;
    }

    sdl2r_del_hash(sdl2r_window_hash, (HASHKEY)win->window);
    SDL_DestroyWindow(win->window);
    win->window = 0;
}


static void sdl2r_window_free(void *ptr)
{
    struct SDL2RWindow *win = ptr;

    if (win->window) {
        sdl2r_window_dispose(win);
    }

    xfree(win);
}


VALUE sdl2r_window_alloc(VALUE klass)
{
    struct SDL2RWindow *win;

    VALUE vwindow = TypedData_Make_Struct(klass, struct SDL2RWindow, &sdl2r_window_data_type, win);
    win->window = 0;
    win->vwindowsurface = Qnil;
    win->vrenderer = Qnil;
    win->vglcontext = Qnil;

    return vwindow;
}


static VALUE sdl2r_window_im_dispose(VALUE self)
{
    sdl2r_window_dispose(SDL2R_GET_WINDOW_STRUCT(self));
    return self;
}


static VALUE sdl2r_window_im_get_disposed(VALUE self)
{
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, self);
    return win->window ? Qfalse : Qtrue;
}


static VALUE sdl2r_destroy_window(VALUE klass, VALUE vwindow)
{
    sdl2r_window_dispose(SDL2R_GET_WINDOW_STRUCT(vwindow));
    return vwindow;
}


VALUE sdl2r_window_sdl_to_ruby(SDL_Window *w)
{
    return sdl2r_get_hash(sdl2r_window_hash, (HASHKEY)w);
}


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

    SDL_SetWindowTitle(win->window, SDL2R_TO_UTF8_PTR(vstr));

    return vstr;
}


static VALUE sdl2r_get_window_surface(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    VALUE vsurface;
    struct SDL2RSurface *sur;

    if (win->vwindowsurface != Qnil) {
        return win->vwindowsurface;
    }

    vsurface = sdl2r_surface_alloc(cSurface);
    sur = SDL2R_GET_STRUCT(Surface, vsurface);

    sur->surface = SDL_GetWindowSurface(win->window);
    if (!sur->surface) {
        rb_raise(eSDLError, SDL_GetError());
    }
    sur->surface = 0;
    sur->vwindow = vwindow;
    win->vwindowsurface = vsurface;

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

    SDL2R_RETRY(win->window = SDL_CreateWindow(RSTRING_PTR(vstr), NUM2INT(vx), NUM2INT(vy), NUM2INT(vw), NUM2INT(vh), NUM2INT(vflags)));
    if (!win->window) {
        rb_raise(eSDLError, SDL_GetError());
    }
    sdl2r_put_hash(sdl2r_window_hash, (HASHKEY)win->window, vwindow);

    return vwindow;
}


static VALUE sdl2r_create_window_and_renderer(VALUE klass, VALUE vw, VALUE vh, VALUE vflags)
{
    VALUE vwindow = sdl2r_window_alloc(cWindow);
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, vwindow);
    VALUE vrenderer = sdl2r_renderer_alloc(cRenderer);
    struct SDL2RRenderer *ren = SDL2R_GET_STRUCT(Renderer, vrenderer);
    int result;

    SDL2R_RETRY((result = SDL_CreateWindowAndRenderer(NUM2INT(vw), NUM2INT(vh), NUM2INT(vflags), &(win->window), &(ren->renderer))) == 0 ? 1 : 0);
    if (result) {
        rb_raise(eSDLError, SDL_GetError());
    }
    sdl2r_put_hash(sdl2r_window_hash, (HASHKEY)win->window, vwindow);
    ren->vwindow = vwindow;
    win->vrenderer = vrenderer;

    return rb_ary_new3(2, vwindow, vrenderer);
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


static VALUE sdl2r_hide_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_HideWindow(win->window);

    return vwindow;
}


static VALUE sdl2r_show_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_ShowWindow(win->window);

    return vwindow;
}


static VALUE sdl2r_get_window_id(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    return UINT2NUM(SDL_GetWindowID(win->window));
}


static VALUE sdl2r_get_window_from_id(VALUE klass, VALUE vid)
{
    SDL_Window *w;

    w = SDL_GetWindowFromID(NUM2UINT(vid));
    if (!w) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return sdl2r_get_hash(sdl2r_window_hash, (HASHKEY)w);
}


static VALUE sdl2r_gl_create_context(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    VALUE vglcontext = sdl2r_glcontext_alloc(cGLContext);
    struct SDL2RGLContext *glc = SDL2R_GET_STRUCT(GLContext, vglcontext);

    SDL2R_RETRY(glc->glcontext = SDL_GL_CreateContext(win->window));
    if (!glc->glcontext) {
        rb_raise(eSDLError, SDL_GetError());
    }
    glc->vwindow = vwindow;
    win->vglcontext = vglcontext;

    return vglcontext;
}


static VALUE sdl2r_get_window_position(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    int x, y;

    SDL_GetWindowPosition(win->window, &x, &y);
    return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}


static VALUE sdl2r_set_window_position(VALUE klass, VALUE vwindow, VALUE vx, VALUE vy)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_SetWindowPosition(win->window, NUM2INT(vx), NUM2INT(vy));

    return vwindow;
}


static VALUE sdl2r_get_window_size(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    int w, h;

    SDL_GetWindowSize(win->window, &w, &h);
    return rb_ary_new3(2, INT2NUM(w), INT2NUM(h));
}


static VALUE sdl2r_set_window_size(VALUE klass, VALUE vwindow, VALUE vw, VALUE vh)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_SetWindowSize(win->window, NUM2INT(vw), NUM2INT(vh));

    return vwindow;
}


static VALUE sdl2r_maximize_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_MaximizeWindow(win->window);
    return vwindow;
}


static VALUE sdl2r_minimize_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_MinimizeWindow(win->window);
    return vwindow;
}


static VALUE sdl2r_restore_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_RestoreWindow(win->window);
    return vwindow;
}



//static VALUE sdl2r_window_test(VALUE klass)
//{
//    printf("n_buckets = %d\n", sdl2r_window_hash->n_buckets);
//    printf("size = %d\n", sdl2r_window_hash->size);
//    printf("n_occupied = %d\n", sdl2r_window_hash->n_occupied);
//    return Qnil;
//}


void Init_sdl2r_window(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(create_window, 6);
    SDL2R_DEFINE_SINGLETON_METHOD(create_window_and_renderer, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(destroy_window, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(create_renderer, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(get_renderer, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(update_window_surface, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_pixel_format, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(hide_window, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(show_window, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(gl_create_context, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_id, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_from_id, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_title, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_window_title, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_position, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_window_position, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(get_window_size, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_window_size, 3);
    SDL2R_DEFINE_SINGLETON_METHOD(maximize_window, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(minimize_window, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(restore_window, 1);

//    rb_define_singleton_method(window_test, 0);

    // SDL::Window class
    cWindow = rb_define_class_under(mSDL, "Window", rb_cObject);
    rb_define_alloc_func(cWindow, sdl2r_window_alloc);

    rb_define_method(cWindow, "dispose", sdl2r_window_im_dispose, 0);
    rb_define_method(cWindow, "disposed?", sdl2r_window_im_get_disposed, 0);

    // Constants
    SDL2R_DEFINE_CONST(WINDOWPOS_CENTERED);
    SDL2R_DEFINE_CONST(WINDOWPOS_UNDEFINED);

    SDL2R_DEFINE_CONST(WINDOW_FULLSCREEN);
    SDL2R_DEFINE_CONST(WINDOW_FULLSCREEN_DESKTOP);
    SDL2R_DEFINE_CONST(WINDOW_OPENGL);
    SDL2R_DEFINE_CONST(WINDOW_SHOWN);
    SDL2R_DEFINE_CONST(WINDOW_HIDDEN);
    SDL2R_DEFINE_CONST(WINDOW_BORDERLESS);
    SDL2R_DEFINE_CONST(WINDOW_RESIZABLE);
    SDL2R_DEFINE_CONST(WINDOW_MINIMIZED);
    SDL2R_DEFINE_CONST(WINDOW_MAXIMIZED);
    SDL2R_DEFINE_CONST(WINDOW_INPUT_GRABBED);
    SDL2R_DEFINE_CONST(WINDOW_INPUT_FOCUS);
    SDL2R_DEFINE_CONST(WINDOW_MOUSE_FOCUS);
    SDL2R_DEFINE_CONST(WINDOW_FOREIGN);
    SDL2R_DEFINE_CONST(WINDOW_ALLOW_HIGHDPI);

    sdl2r_window_hash = sdl2r_hash_alloc(8);
}
