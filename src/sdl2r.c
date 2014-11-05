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

// global object
VALUE g_enum_array;


// enum
static VALUE sdl2r_EnumPixelFormat;
static VALUE sdl2r_EnumPixelType;
static VALUE sdl2r_EnumBitmapOrder;
static VALUE sdl2r_EnumPackOrder;
static VALUE sdl2r_EnumArrayOrder;
static VALUE sdl2r_EnumPackedLayout;
static VALUE sdl2r_EnumBlendMode;
static VALUE sdl2r_EnumEndian;

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
    SDL2R_CLEAR_HASH(sdl2r_window_hash, Window, window, sdl2r_window_dispose);
    SDL2R_CLEAR_HASH(sdl2r_joystick_hash, Joystick, joystick, sdl2r_joystick_dispose);
    SDL2R_CLEAR_HASH(sdl2r_chunk_hash, Chunk, chunk, sdl2r_chunk_dispose);
    SDL2R_CLEAR_HASH(sdl2r_music_hash, Music, music, sdl2r_music_dispose);
    SDL2R_CLEAR_HASH(sdl2r_font_hash, Font, font, sdl2r_font_dispose);
    rb_hash_clear(g_user_event_data);
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


static VALUE sdl2r_macro_PIXELTYPE(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_PIXELTYPE(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_PIXELORDER(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_PIXELORDER(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_PIXELLAYOUT(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_PIXELLAYOUT(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_BITSPERPIXEL(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_BITSPERPIXEL(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_BYTESPERPIXEL(VALUE klass, VALUE vformat)
{
    return INT2FIX(SDL_BYTESPERPIXEL(NUM2INT(vformat)));
}
static VALUE sdl2r_macro_ISPIXELFORMAT_INDEXED(VALUE klass, VALUE vformat)
{
    return SDL_ISPIXELFORMAT_INDEXED(NUM2INT(vformat)) ? Qtrue : Qfalse;
}
static VALUE sdl2r_macro_ISPIXELFORMAT_ALPHA(VALUE klass, VALUE vformat)
{
    return SDL_ISPIXELFORMAT_ALPHA(NUM2INT(vformat)) ? Qtrue : Qfalse;
}
static VALUE sdl2r_macro_ISPIXELFORMAT_FOURCC(VALUE klass, VALUE vformat)
{
    return SDL_ISPIXELFORMAT_FOURCC(NUM2INT(vformat)) ? Qtrue : Qfalse;
}


// etc.
static void sdl2r_shutdown(VALUE obj)
{
    SDL2R_CLEAR_HASH(sdl2r_window_hash, Window, window, sdl2r_window_dispose);
    SDL2R_CLEAR_HASH(sdl2r_joystick_hash, Joystick, joystick, sdl2r_joystick_dispose);
    SDL2R_CLEAR_HASH(sdl2r_chunk_hash, Chunk, chunk, sdl2r_chunk_dispose);
    SDL2R_CLEAR_HASH(sdl2r_music_hash, Music, music, sdl2r_music_dispose);
    SDL2R_CLEAR_HASH(sdl2r_font_hash, Font, font, sdl2r_font_dispose);
}


void Init_sdl2r(void)
{
    mSDL = rb_define_module("SDL");

    // exception
    eSDLError = rb_define_class_under(mSDL, "SDLError", rb_eRuntimeError);
    eSDL2RError = rb_define_class_under(mSDL, "SDL2RError", rb_eRuntimeError);

    // init and quit
    SDL2R_DEFINE_SINGLETON_METHOD(init, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(init_sub_system, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(quit, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(quit_sub_system, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(set_main_ready, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(was_init, 1);

    // shutdown
    rb_set_end_proc(sdl2r_shutdown, Qnil);

    // error
    SDL2R_DEFINE_SINGLETON_METHOD(clear_error, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(get_error, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(set_error, 1);

    // enum array
    g_enum_array = rb_ary_new();
    rb_global_variable(&g_enum_array);

    // Constants
    SDL2R_DEFINE_CONST(INIT_TIMER);
    SDL2R_DEFINE_CONST(INIT_AUDIO);
    SDL2R_DEFINE_CONST(INIT_VIDEO);
    SDL2R_DEFINE_CONST(INIT_JOYSTICK);
    SDL2R_DEFINE_CONST(INIT_HAPTIC);
    SDL2R_DEFINE_CONST(INIT_GAMECONTROLLER);
    SDL2R_DEFINE_CONST(INIT_EVENTS);
    SDL2R_DEFINE_CONST(INIT_EVERYTHING);
    SDL2R_DEFINE_CONST(INIT_NOPARACHUTE);

    SDL2R_DEFINE_CONST(QUERY);
    SDL2R_DEFINE_CONST(IGNORE);
    SDL2R_DEFINE_CONST(DISABLE);
    SDL2R_DEFINE_CONST(ENABLE);

    // enum
    SDL2R_DEFINE_ENUM(EnumPixelFormat);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX1LSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX1MSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX4LSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX4MSB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_INDEX8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB332);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBA4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ABGR4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRA4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB1555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBA5551);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ABGR1555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRA5551);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB565);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR565);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB24);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR24);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGB888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBX8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGR888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRX8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_RGBA8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ABGR8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_BGRA8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_ARGB2101010);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_YV12);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_IYUV);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_YUY2);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_UYVY);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelFormat, PIXELFORMAT_YVYU);

    SDL2R_DEFINE_ENUM(EnumPixelType);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_INDEX1);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_INDEX4);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_INDEX8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_PACKED8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_PACKED16);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_PACKED32);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYU8);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYU16);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYU32);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYF16);
    SDL2R_DEFINE_ENUM_VALUE(EnumPixelType, PIXELTYPE_ARRAYF32);

    SDL2R_DEFINE_ENUM(EnumBitmapOrder);
    SDL2R_DEFINE_ENUM_VALUE(EnumBitmapOrder, BITMAPORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumBitmapOrder, BITMAPORDER_4321);
    SDL2R_DEFINE_ENUM_VALUE(EnumBitmapOrder, BITMAPORDER_1234);

    SDL2R_DEFINE_ENUM(EnumPackOrder);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_XRGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_RGBX);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_ARGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_RGBA);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_XBGR);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_BGRX);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_ABGR);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackOrder, PACKEDORDER_BGRA);

    SDL2R_DEFINE_ENUM(EnumArrayOrder);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_RGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_RGBA);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_ARGB);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_BGR);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_BGRA);
    SDL2R_DEFINE_ENUM_VALUE(EnumArrayOrder, ARRAYORDER_ABGR);

    SDL2R_DEFINE_ENUM(EnumPackedLayout);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_332);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_4444);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_1555);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_5551);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_565);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_8888);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_2101010);
    SDL2R_DEFINE_ENUM_VALUE(EnumPackedLayout, PACKEDLAYOUT_1010102);

    SDL2R_DEFINE_ENUM(EnumBlendMode);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_NONE);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_BLEND);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_ADD);
    SDL2R_DEFINE_ENUM_VALUE(EnumBlendMode, BLENDMODE_MOD);

    SDL2R_DEFINE_ENUM(EnumEndian);
    SDL2R_DEFINE_ENUM_VALUE(EnumEndian, BYTEORDER);
    SDL2R_DEFINE_ENUM_VALUE(EnumEndian, BIG_ENDIAN);
    SDL2R_DEFINE_ENUM_VALUE(EnumEndian, LIL_ENDIAN);

    // SDL macro
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(PIXELTYPE, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(PIXELORDER, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(PIXELLAYOUT, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(BITSPERPIXEL, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(BYTESPERPIXEL, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(ISPIXELFORMAT_INDEXED, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(ISPIXELFORMAT_ALPHA, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MACRO(ISPIXELFORMAT_FOURCC, 1);

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
