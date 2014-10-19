#include <SDL.h>

#define SDL2R
#include "sdl2r.h"
#include "sdl2r_window.h"
#include "sdl2r_surface.h"
#include "sdl2r_renderer.h"
#include "sdl2r_event.h"
#include "sdl2r_texture.h"
#include "sdl2r_timer.h"
#include "sdl2r_mixer.h"
#include "sdl2r_image.h"
#include "sdl2r_ttf.h"

VALUE mSDL;
VALUE eSDLError;
VALUE eSDL2RError;
VALUE cPoint;
VALUE cRect;
VALUE cColor;

// encoding
rb_encoding *g_enc_utf8;

// init and quit
static VALUE sdl2r_init(VALUE klass, VALUE flags)
{
    return INT2FIX(SDL_Init(NUM2INT(flags)));
}


static VALUE sdl2r_init_sub_system(VALUE klass, VALUE flags)
{
    return INT2FIX(SDL_InitSubSystem(NUM2INT(flags)));
}


static VALUE sdl2r_quit(VALUE klass)
{
    SDL_Quit();
    return Qnil;
}


static VALUE sdl2r_quit_sub_system(VALUE klass, VALUE flags)
{
    SDL_QuitSubSystem(NUM2INT(flags));
    return Qnil;
}


static VALUE sdl2r_set_main_ready(VALUE klass)
{
    SDL_SetMainReady();
    return Qnil;
}


static VALUE sdl2r_was_init(VALUE klass, VALUE flags)
{
    return INT2FIX(SDL_WasInit(NUM2INT(flags)));
}


// error
static VALUE sdl2r_clear_error(VALUE klass)
{
    SDL_ClearError();
    return Qnil;
}


static VALUE sdl2r_get_error(VALUE klass)
{
    return rb_str_new2(SDL_GetError());
}


static VALUE sdl2r_set_error(VALUE klass, VALUE vstr)
{
    return INT2FIX(SDL_SetError(RSTRING_PTR(vstr)));
}


static VALUE sdl2r_pixeltype(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_PIXELTYPE(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_pixelorder(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_PIXELORDER(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_pixellayout(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_PIXELLAYOUT(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_bitsperpixel(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_BITSPERPIXEL(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_bytesperpixel(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_BYTESPERPIXEL(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_ispixelformat_indexed(VALUE klass, VALUE vformat)
{
    return SDL_ISPIXELFORMAT_INDEXED(NUM2INT(vformat)) ? Qtrue : Qfalse;
}
static VALUE sdl2r_macro_ispixelformat_alpha(VALUE klass, VALUE vformat)
{
    return SDL_ISPIXELFORMAT_ALPHA(NUM2INT(vformat)) ? Qtrue : Qfalse;
}
static VALUE sdl2r_macro_ispixelformat_fourcc(VALUE klass, VALUE vformat)
{
    return SDL_ISPIXELFORMAT_FOURCC(NUM2INT(vformat)) ? Qtrue : Qfalse;
}


// etc.
static void sdl2r_shutdown(VALUE obj)
{
}


void Init_sdl2r(void)
{
    mSDL = rb_define_module("SDL");

    // exception
    eSDLError = rb_define_class_under(mSDL, "SDLError", rb_eRuntimeError);
    eSDL2RError = rb_define_class_under(mSDL, "SDL2RError", rb_eRuntimeError);

    // init and quit
    rb_define_singleton_method(mSDL, "init", sdl2r_init, 1);
    rb_define_singleton_method(mSDL, "init_sub_system", sdl2r_init_sub_system, 1);
    rb_define_singleton_method(mSDL, "quit", sdl2r_quit, 0);
    rb_define_singleton_method(mSDL, "quit_sub_system", sdl2r_quit_sub_system, 1);
    rb_define_singleton_method(mSDL, "set_main_ready", sdl2r_set_main_ready, 0);
    rb_define_singleton_method(mSDL, "was_init", sdl2r_was_init, 1);

    rb_set_end_proc(sdl2r_shutdown, Qnil);

    // Constants
    rb_define_const(mSDL, "INIT_TIMER", INT2FIX(SDL_INIT_TIMER));
    rb_define_const(mSDL, "INIT_AUDIO", INT2FIX(SDL_INIT_AUDIO));
    rb_define_const(mSDL, "INIT_VIDEO", INT2FIX(SDL_INIT_VIDEO));
    rb_define_const(mSDL, "INIT_JOYSTICK", INT2FIX(SDL_INIT_JOYSTICK));
    rb_define_const(mSDL, "INIT_HAPTIC", INT2FIX(SDL_INIT_HAPTIC));
    rb_define_const(mSDL, "INIT_GAMECONTROLLER", INT2FIX(SDL_INIT_GAMECONTROLLER));
    rb_define_const(mSDL, "INIT_EVENTS", INT2FIX(SDL_INIT_EVENTS));
    rb_define_const(mSDL, "INIT_EVERYTHING", INT2FIX(SDL_INIT_EVERYTHING));
    rb_define_const(mSDL, "INIT_NOPARACHUTE", INT2FIX(SDL_INIT_NOPARACHUTE));

    // error
    rb_define_singleton_method(mSDL, "clear_error", sdl2r_clear_error, 0);
    rb_define_singleton_method(mSDL, "get_error", sdl2r_get_error, 0);
    rb_define_singleton_method(mSDL, "set_error", sdl2r_set_error, 1);

    // Constants
    rb_define_const(mSDL, "PIXELFORMAT_UNKNOWN", INT2FIX(SDL_PIXELFORMAT_UNKNOWN));
    rb_define_const(mSDL, "PIXELFORMAT_INDEX1LSB", INT2FIX(SDL_PIXELFORMAT_INDEX1LSB));
    rb_define_const(mSDL, "PIXELFORMAT_INDEX1MSB", INT2FIX(SDL_PIXELFORMAT_INDEX1MSB));
    rb_define_const(mSDL, "PIXELFORMAT_INDEX4LSB", INT2FIX(SDL_PIXELFORMAT_INDEX4LSB));
    rb_define_const(mSDL, "PIXELFORMAT_INDEX4MSB", INT2FIX(SDL_PIXELFORMAT_INDEX4MSB));
    rb_define_const(mSDL, "PIXELFORMAT_INDEX8", INT2FIX(SDL_PIXELFORMAT_INDEX8));
    rb_define_const(mSDL, "PIXELFORMAT_RGB332", INT2FIX(SDL_PIXELFORMAT_RGB332));
    rb_define_const(mSDL, "PIXELFORMAT_RGB444", INT2FIX(SDL_PIXELFORMAT_RGB444));
    rb_define_const(mSDL, "PIXELFORMAT_RGB555", INT2FIX(SDL_PIXELFORMAT_RGB555));
    rb_define_const(mSDL, "PIXELFORMAT_BGR555", INT2FIX(SDL_PIXELFORMAT_BGR555));
    rb_define_const(mSDL, "PIXELFORMAT_ARGB4444", INT2FIX(SDL_PIXELFORMAT_ARGB4444));
    rb_define_const(mSDL, "PIXELFORMAT_RGBA4444", INT2FIX(SDL_PIXELFORMAT_RGBA4444));
    rb_define_const(mSDL, "PIXELFORMAT_ABGR4444", INT2FIX(SDL_PIXELFORMAT_ABGR4444));
    rb_define_const(mSDL, "PIXELFORMAT_BGRA4444", INT2FIX(SDL_PIXELFORMAT_BGRA4444));
    rb_define_const(mSDL, "PIXELFORMAT_ARGB1555", INT2FIX(SDL_PIXELFORMAT_ARGB1555));
    rb_define_const(mSDL, "PIXELFORMAT_RGBA5551", INT2FIX(SDL_PIXELFORMAT_RGBA5551));
    rb_define_const(mSDL, "PIXELFORMAT_ABGR1555", INT2FIX(SDL_PIXELFORMAT_ABGR1555));
    rb_define_const(mSDL, "PIXELFORMAT_BGRA5551", INT2FIX(SDL_PIXELFORMAT_BGRA5551));
    rb_define_const(mSDL, "PIXELFORMAT_RGB565", INT2FIX(SDL_PIXELFORMAT_RGB565));
    rb_define_const(mSDL, "PIXELFORMAT_BGR565", INT2FIX(SDL_PIXELFORMAT_BGR565));
    rb_define_const(mSDL, "PIXELFORMAT_RGB24", INT2FIX(SDL_PIXELFORMAT_RGB24));
    rb_define_const(mSDL, "PIXELFORMAT_BGR24", INT2FIX(SDL_PIXELFORMAT_BGR24));
    rb_define_const(mSDL, "PIXELFORMAT_RGB888", INT2FIX(SDL_PIXELFORMAT_RGB888));
    rb_define_const(mSDL, "PIXELFORMAT_RGBX8888", INT2FIX(SDL_PIXELFORMAT_RGBX8888));
    rb_define_const(mSDL, "PIXELFORMAT_BGR888", INT2FIX(SDL_PIXELFORMAT_BGR888));
    rb_define_const(mSDL, "PIXELFORMAT_BGRX8888", INT2FIX(SDL_PIXELFORMAT_BGRX8888));
    rb_define_const(mSDL, "PIXELFORMAT_ARGB8888", INT2FIX(SDL_PIXELFORMAT_ARGB8888));
    rb_define_const(mSDL, "PIXELFORMAT_RGBA8888", INT2FIX(SDL_PIXELFORMAT_RGBA8888));
    rb_define_const(mSDL, "PIXELFORMAT_ABGR8888", INT2FIX(SDL_PIXELFORMAT_ABGR8888));
    rb_define_const(mSDL, "PIXELFORMAT_BGRA8888", INT2FIX(SDL_PIXELFORMAT_BGRA8888));
    rb_define_const(mSDL, "PIXELFORMAT_ARGB2101010", INT2FIX(SDL_PIXELFORMAT_ARGB2101010));
    rb_define_const(mSDL, "PIXELFORMAT_YV12", INT2FIX(SDL_PIXELFORMAT_YV12));
    rb_define_const(mSDL, "PIXELFORMAT_IYUV", INT2FIX(SDL_PIXELFORMAT_IYUV));
    rb_define_const(mSDL, "PIXELFORMAT_YUY2", INT2FIX(SDL_PIXELFORMAT_YUY2));
    rb_define_const(mSDL, "PIXELFORMAT_UYVY", INT2FIX(SDL_PIXELFORMAT_UYVY));
    rb_define_const(mSDL, "PIXELFORMAT_YVYU", INT2FIX(SDL_PIXELFORMAT_YVYU));

    rb_define_const(mSDL, "PIXELTYPE_UNKNOWN", INT2FIX(SDL_PIXELTYPE_UNKNOWN));
    rb_define_const(mSDL, "PIXELTYPE_INDEX1", INT2FIX(SDL_PIXELTYPE_INDEX1));
    rb_define_const(mSDL, "PIXELTYPE_INDEX4", INT2FIX(SDL_PIXELTYPE_INDEX4));
    rb_define_const(mSDL, "PIXELTYPE_INDEX8", INT2FIX(SDL_PIXELTYPE_INDEX8));
    rb_define_const(mSDL, "PIXELTYPE_PACKED8", INT2FIX(SDL_PIXELTYPE_PACKED8));
    rb_define_const(mSDL, "PIXELTYPE_PACKED16", INT2FIX(SDL_PIXELTYPE_PACKED16));
    rb_define_const(mSDL, "PIXELTYPE_PACKED32", INT2FIX(SDL_PIXELTYPE_PACKED32));
    rb_define_const(mSDL, "PIXELTYPE_ARRAYU8", INT2FIX(SDL_PIXELTYPE_ARRAYU8));
    rb_define_const(mSDL, "PIXELTYPE_ARRAYU16", INT2FIX(SDL_PIXELTYPE_ARRAYU16));
    rb_define_const(mSDL, "PIXELTYPE_ARRAYU32", INT2FIX(SDL_PIXELTYPE_ARRAYU32));
    rb_define_const(mSDL, "PIXELTYPE_ARRAYF16", INT2FIX(SDL_PIXELTYPE_ARRAYF16));
    rb_define_const(mSDL, "PIXELTYPE_ARRAYF32", INT2FIX(SDL_PIXELTYPE_ARRAYF32));

    rb_define_const(mSDL, "BITMAPORDER_NONE", INT2FIX(SDL_BITMAPORDER_NONE));
    rb_define_const(mSDL, "BITMAPORDER_4321", INT2FIX(SDL_BITMAPORDER_4321));
    rb_define_const(mSDL, "BITMAPORDER_1234", INT2FIX(SDL_BITMAPORDER_1234));

    rb_define_const(mSDL, "PACKEDORDER_NONE", INT2FIX(SDL_PACKEDORDER_NONE));
    rb_define_const(mSDL, "PACKEDORDER_XRGB", INT2FIX(SDL_PACKEDORDER_XRGB));
    rb_define_const(mSDL, "PACKEDORDER_RGBX", INT2FIX(SDL_PACKEDORDER_RGBX));
    rb_define_const(mSDL, "PACKEDORDER_ARGB", INT2FIX(SDL_PACKEDORDER_ARGB));
    rb_define_const(mSDL, "PACKEDORDER_RGBA", INT2FIX(SDL_PACKEDORDER_RGBA));
    rb_define_const(mSDL, "PACKEDORDER_XBGR", INT2FIX(SDL_PACKEDORDER_XBGR));
    rb_define_const(mSDL, "PACKEDORDER_BGRX", INT2FIX(SDL_PACKEDORDER_BGRX));
    rb_define_const(mSDL, "PACKEDORDER_ABGR", INT2FIX(SDL_PACKEDORDER_ABGR));
    rb_define_const(mSDL, "PACKEDORDER_BGRA", INT2FIX(SDL_PACKEDORDER_BGRA));

    rb_define_const(mSDL, "ARRAYORDER_NONE", INT2FIX(SDL_ARRAYORDER_NONE));
    rb_define_const(mSDL, "ARRAYORDER_RGB", INT2FIX(SDL_ARRAYORDER_RGB));
    rb_define_const(mSDL, "ARRAYORDER_RGBA", INT2FIX(SDL_ARRAYORDER_RGBA));
    rb_define_const(mSDL, "ARRAYORDER_ARGB", INT2FIX(SDL_ARRAYORDER_ARGB));
    rb_define_const(mSDL, "ARRAYORDER_BGR", INT2FIX(SDL_ARRAYORDER_BGR));
    rb_define_const(mSDL, "ARRAYORDER_BGRA", INT2FIX(SDL_ARRAYORDER_BGRA));
    rb_define_const(mSDL, "ARRAYORDER_ABGR", INT2FIX(SDL_ARRAYORDER_ABGR));

    rb_define_const(mSDL, "PACKEDLAYOUT_NONE", INT2FIX(SDL_PACKEDLAYOUT_NONE));
    rb_define_const(mSDL, "PACKEDLAYOUT_332", INT2FIX(SDL_PACKEDLAYOUT_332));
    rb_define_const(mSDL, "PACKEDLAYOUT_4444", INT2FIX(SDL_PACKEDLAYOUT_4444));
    rb_define_const(mSDL, "PACKEDLAYOUT_1555", INT2FIX(SDL_PACKEDLAYOUT_1555));
    rb_define_const(mSDL, "PACKEDLAYOUT_5551", INT2FIX(SDL_PACKEDLAYOUT_5551));
    rb_define_const(mSDL, "PACKEDLAYOUT_565", INT2FIX(SDL_PACKEDLAYOUT_565));
    rb_define_const(mSDL, "PACKEDLAYOUT_8888", INT2FIX(SDL_PACKEDLAYOUT_8888));
    rb_define_const(mSDL, "PACKEDLAYOUT_2101010", INT2FIX(SDL_PACKEDLAYOUT_2101010));
    rb_define_const(mSDL, "PACKEDLAYOUT_1010102", INT2FIX(SDL_PACKEDLAYOUT_1010102));

    rb_define_const(mSDL, "BLENDMODE_NONE", INT2FIX(SDL_BLENDMODE_NONE));
    rb_define_const(mSDL, "BLENDMODE_BLEND", INT2FIX(SDL_BLENDMODE_BLEND));
    rb_define_const(mSDL, "BLENDMODE_ADD", INT2FIX(SDL_BLENDMODE_ADD));
    rb_define_const(mSDL, "BLENDMODE_MOD", INT2FIX(SDL_BLENDMODE_MOD));

    rb_define_const(mSDL, "BYTEORDER", INT2FIX(SDL_BYTEORDER));
    rb_define_const(mSDL, "BIG_ENDIAN", INT2FIX(SDL_BIG_ENDIAN));
    rb_define_const(mSDL, "LIL_ENDIAN", INT2FIX(SDL_LIL_ENDIAN));

    // SDL macro
    rb_define_singleton_method(mSDL, "PIXELTYPE", sdl2r_pixeltype, 1);
    rb_define_singleton_method(mSDL, "PIXELORDER", sdl2r_macro_pixelorder, 1);
    rb_define_singleton_method(mSDL, "PIXELLAYOUT", sdl2r_macro_pixellayout, 1);
    rb_define_singleton_method(mSDL, "BITSPERPIXEL", sdl2r_macro_bitsperpixel, 1);
    rb_define_singleton_method(mSDL, "BYTESPERPIXEL", sdl2r_macro_bytesperpixel, 1);
    rb_define_singleton_method(mSDL, "ISPIXELFORMAT_INDEXED", sdl2r_macro_ispixelformat_indexed, 1);
    rb_define_singleton_method(mSDL, "ISPIXELFORMAT_ALPHA", sdl2r_macro_ispixelformat_alpha, 1);
    rb_define_singleton_method(mSDL, "ISPIXELFORMAT_FOURCC", sdl2r_macro_ispixelformat_fourcc, 1);

    // Encoding data
    g_enc_utf8 = rb_enc_find("UTF-8");

    // SDL::Point class
    cPoint = rb_define_class_under(mSDL, "Point", rb_cArray);

    // SDL::Rect class
    cRect = rb_define_class_under(mSDL, "Rect", rb_cArray);

    // SDL::Color class
    cColor = rb_define_class_under(mSDL, "Color", rb_cArray);

    // initialize
    Init_sdl2r_window();
    Init_sdl2r_surface();
    Init_sdl2r_renderer();
    Init_sdl2r_texture();
    Init_sdl2r_event();
    Init_sdl2r_timer();
    Init_sdl2r_mixer();
    Init_sdl2r_image();
    Init_sdl2r_ttf();
}


