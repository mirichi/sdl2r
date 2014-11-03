#define SDL2ROPENGL
#include "sdl2r.h"
#include "sdl2r_opengl.h"
#include "sdl2r_window.h"

VALUE cGLContext;

static void sdl2r_glcontext_free(void *ptr);
static void sdl2r_glcontext_mark(void *ptr);

const rb_data_type_t sdl2r_glcontext_data_type = {
    "GLContext",
    {
    sdl2r_glcontext_mark,
    sdl2r_glcontext_free,
    0,
    },
};


void sdl2r_glcontext_dispose(struct SDL2RGLContext *glc)
{
    struct SDL2RWindow *win = SDL2R_GET_STRUCT(Window, glc->vwindow);
    SDL_GL_DeleteContext(glc->glcontext);
    glc->glcontext = 0;
    glc->vwindow = Qnil;
    win->vglcontext = Qnil;
}


static void sdl2r_glcontext_free(void *ptr)
{
    struct SDL2RGLContext *glc = ptr;

    if (glc->glcontext) {
        sdl2r_glcontext_dispose(glc);
    }

    xfree(glc);
}


static void sdl2r_glcontext_mark(void *ptr)
{
    struct SDL2RGLContext *glc = ptr;
    rb_gc_mark(glc->vwindow);
}


VALUE sdl2r_glcontext_alloc(VALUE klass)
{
    struct SDL2RGLContext *glc;
    VALUE vglcontext = TypedData_Make_Struct(klass, struct SDL2RGLContext, &sdl2r_glcontext_data_type, glc);
    glc->glcontext = 0;
    glc->vwindow = Qnil;
    return vglcontext;
}


static VALUE sdl2r_glcontext_im_dispose(VALUE self)
{
    sdl2r_glcontext_dispose(SDL2R_GET_GLCONTEXT_STRUCT(self));
    return self;
}


static VALUE sdl2r_glcontext_im_get_disposed(VALUE self)
{
    struct SDL2RGLContext *glc = SDL2R_GET_STRUCT(GLContext, self);

    return glc->glcontext ? Qfalse : Qtrue;
}


static VALUE sdl2r_gl_delete_context(VALUE klass, VALUE vglcontext)
{
    sdl2r_glcontext_dispose(SDL2R_GET_GLCONTEXT_STRUCT(vglcontext));
    return vglcontext;
}


static VALUE sdl2r_gl_make_current(VALUE klass, VALUE vwindow, VALUE vglcontext)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);
    struct SDL2RGLContext *glc = SDL2R_GET_GLCONTEXT_STRUCT(vglcontext);

    if (SDL_GL_MakeCurrent(win->window, glc->glcontext)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vglcontext;
}


static VALUE sdl2r_gl_swap_window(VALUE klass, VALUE vwindow)
{
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    SDL_GL_SwapWindow(win->window);

    return vwindow;
}


void Init_sdl2r_opengl(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(gl_delete_context, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_extensionSupported, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_getAttribute, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_getCurglctContext, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_getCurglctWindow, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_getDrawableSize, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_getProcAddress, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_getSwapInterval, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_loadLibrary, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(gl_make_current, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_resetAttributes, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_setAttribute, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_setSwapInterval, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(gl_swap_window, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(gl_unloadLibrary, 0);

    // SDL::GLContext class
    cGLContext = rb_define_class_under(mSDL, "GLContext", rb_cObject);
    rb_define_alloc_func(cGLContext, sdl2r_glcontext_alloc);

    rb_define_method(cGLContext, "dispose", sdl2r_glcontext_im_dispose, 0);
    rb_define_method(cGLContext, "disposed?", sdl2r_glcontext_im_get_disposed, 0);
}


