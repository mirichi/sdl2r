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


static VALUE sdl2r_gl_delete_glcontext(VALUE klass, VALUE vglcontext)
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
    rb_define_singleton_method(mSDL, "gl_delete_context", sdl2r_gl_delete_glcontext, 1);
//    rb_define_singleton_method(mSDL, "gl_extensionSupported
//    rb_define_singleton_method(mSDL, "gl_getAttribute
//    rb_define_singleton_method(mSDL, "gl_getCurglctContext
//    rb_define_singleton_method(mSDL, "gl_getCurglctWindow
//    rb_define_singleton_method(mSDL, "gl_getDrawableSize
//    rb_define_singleton_method(mSDL, "gl_getProcAddress
//    rb_define_singleton_method(mSDL, "gl_getSwapInterval
//    rb_define_singleton_method(mSDL, "gl_loadLibrary
    rb_define_singleton_method(mSDL, "gl_make_current", sdl2r_gl_make_current, 2);
//    rb_define_singleton_method(mSDL, "gl_resetAttributes
//    rb_define_singleton_method(mSDL, "gl_setAttribute
//    rb_define_singleton_method(mSDL, "gl_setSwapInterval
    rb_define_singleton_method(mSDL, "gl_swap_window", sdl2r_gl_swap_window, 1);
//    rb_define_singleton_method(mSDL, "gl_unloadLibrary

    // SDL::GLContext class
    cGLContext = rb_define_class_under(mSDL, "GLContext", rb_cObject);
    rb_define_alloc_func(cGLContext, sdl2r_glcontext_alloc);

    rb_define_method(cGLContext, "dispose", sdl2r_glcontext_im_dispose, 0);
    rb_define_method(cGLContext, "disposed?", sdl2r_glcontext_im_get_disposed, 0);
}


