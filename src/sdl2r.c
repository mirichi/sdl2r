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
#include "sdl2r_joystick.h"
#include "sdl2r_filesystem.h"
#include "sdl2r_rwops.h"
#include "sdl2r_keyboard.h"
#include "sdl2r_clipboard.h"
#include "sdl2r_opengl.h"
#include "sdl2r_mouse.h"
#include "sdl2r_rect.h"
#include "sdl2r_version.h"

VALUE mSDL;
VALUE eSDLError;
VALUE eSDL2RError;
VALUE cColor;

// encoding
rb_encoding *g_enc_utf8;
rb_encoding *g_enc_utf16;

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

    // error
    rb_define_singleton_method(mSDL, "clear_error", sdl2r_clear_error, 0);
    rb_define_singleton_method(mSDL, "get_error", sdl2r_get_error, 0);
    rb_define_singleton_method(mSDL, "set_error", sdl2r_set_error, 1);

    // Constants
    SDL2R_DEFINE_CONST(mSDL, INIT_TIMER);
    SDL2R_DEFINE_CONST(mSDL, INIT_AUDIO);
    SDL2R_DEFINE_CONST(mSDL, INIT_VIDEO);
    SDL2R_DEFINE_CONST(mSDL, INIT_JOYSTICK);
    SDL2R_DEFINE_CONST(mSDL, INIT_HAPTIC);
    SDL2R_DEFINE_CONST(mSDL, INIT_GAMECONTROLLER);
    SDL2R_DEFINE_CONST(mSDL, INIT_EVENTS);
    SDL2R_DEFINE_CONST(mSDL, INIT_EVERYTHING);
    SDL2R_DEFINE_CONST(mSDL, INIT_NOPARACHUTE);

    // Constants
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_UNKNOWN);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_INDEX1LSB);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_INDEX1MSB);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_INDEX4LSB);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_INDEX4MSB);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_INDEX8);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGB332);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGB444);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGB555);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGR555);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ARGB4444);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGBA4444);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ABGR4444);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGRA4444);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ARGB1555);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGBA5551);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ABGR1555);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGRA5551);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGB565);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGR565);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGB24);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGR24);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGB888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGBX8888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGR888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGRX8888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ARGB8888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_RGBA8888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ABGR8888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_BGRA8888);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_ARGB2101010);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_YV12);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_IYUV);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_YUY2);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_UYVY);
    SDL2R_DEFINE_CONST(mSDL, PIXELFORMAT_YVYU);

    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_UNKNOWN);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_INDEX1);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_INDEX4);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_INDEX8);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_PACKED8);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_PACKED16);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_PACKED32);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_ARRAYU8);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_ARRAYU16);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_ARRAYU32);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_ARRAYF16);
    SDL2R_DEFINE_CONST(mSDL, PIXELTYPE_ARRAYF32);

    SDL2R_DEFINE_CONST(mSDL, BITMAPORDER_NONE);
    SDL2R_DEFINE_CONST(mSDL, BITMAPORDER_4321);
    SDL2R_DEFINE_CONST(mSDL, BITMAPORDER_1234);

    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_NONE);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_XRGB);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_RGBX);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_ARGB);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_RGBA);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_XBGR);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_BGRX);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_ABGR);
    SDL2R_DEFINE_CONST(mSDL, PACKEDORDER_BGRA);

    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_NONE);
    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_RGB);
    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_RGBA);
    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_ARGB);
    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_BGR);
    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_BGRA);
    SDL2R_DEFINE_CONST(mSDL, ARRAYORDER_ABGR);

    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_NONE);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_332);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_4444);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_1555);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_5551);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_565);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_8888);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_2101010);
    SDL2R_DEFINE_CONST(mSDL, PACKEDLAYOUT_1010102);

    SDL2R_DEFINE_CONST(mSDL, BLENDMODE_NONE);
    SDL2R_DEFINE_CONST(mSDL, BLENDMODE_BLEND);
    SDL2R_DEFINE_CONST(mSDL, BLENDMODE_ADD);
    SDL2R_DEFINE_CONST(mSDL, BLENDMODE_MOD);

    SDL2R_DEFINE_CONST(mSDL, BYTEORDER);
    SDL2R_DEFINE_CONST(mSDL, BIG_ENDIAN);
    SDL2R_DEFINE_CONST(mSDL, LIL_ENDIAN);

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
    g_enc_utf16 = rb_enc_find("UTF-16LE");

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
    Init_sdl2r_joystick();
    Init_sdl2r_filesystem();
    Init_sdl2r_rwops();
    Init_sdl2r_keyboard();
    Init_sdl2r_clipboard();
    Init_sdl2r_opengl();
    Init_sdl2r_mouse();
    Init_sdl2r_rect();
    Init_sdl2r_version();
}


