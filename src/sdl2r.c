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

// global array
VALUE g_global_array;

// enum
static VALUE sdl2r_enum_PixelFormatEnum;
static VALUE sdl2r_enum_PixelTypeEnum;
static VALUE sdl2r_enum_BitmapOrderEnum;
static VALUE sdl2r_enum_PackOrderEnum;
static VALUE sdl2r_enum_ArrayOrderEnum;
static VALUE sdl2r_enum_PackedLayoutEnum;
static VALUE sdl2r_enum_BlendModeEnum;
static VALUE sdl2r_enum_EndianEnum;

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

    // global array
    g_global_array = rb_ary_new();
    rb_global_variable(&g_global_array);

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
    SDL2R_DEFINE_ENUM(PixelFormatEnum);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_INDEX1LSB);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_INDEX1MSB);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_INDEX4LSB);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_INDEX4MSB);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_INDEX8);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGB332);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGB444);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGB555);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGR555);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ARGB4444);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGBA4444);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ABGR4444);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGRA4444);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ARGB1555);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGBA5551);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ABGR1555);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGRA5551);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGB565);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGR565);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGB24);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGR24);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGB888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGBX8888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGR888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGRX8888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ARGB8888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_RGBA8888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ABGR8888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_BGRA8888);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_ARGB2101010);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_YV12);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_IYUV);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_YUY2);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_UYVY);
    SDL2R_DEFINE_ENUM_VALUE(PixelFormatEnum, PIXELFORMAT_YVYU);

    SDL2R_DEFINE_ENUM(PixelTypeEnum);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_INDEX1);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_INDEX4);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_INDEX8);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_PACKED8);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_PACKED16);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_PACKED32);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_ARRAYU8);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_ARRAYU16);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_ARRAYU32);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_ARRAYF16);
    SDL2R_DEFINE_ENUM_VALUE(PixelTypeEnum, PIXELTYPE_ARRAYF32);

    SDL2R_DEFINE_ENUM(BitmapOrderEnum);
    SDL2R_DEFINE_ENUM_VALUE(BitmapOrderEnum, BITMAPORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(BitmapOrderEnum, BITMAPORDER_4321);
    SDL2R_DEFINE_ENUM_VALUE(BitmapOrderEnum, BITMAPORDER_1234);

    SDL2R_DEFINE_ENUM(PackOrderEnum);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_XRGB);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_RGBX);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_ARGB);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_RGBA);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_XBGR);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_BGRX);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_ABGR);
    SDL2R_DEFINE_ENUM_VALUE(PackOrderEnum, PACKEDORDER_BGRA);

    SDL2R_DEFINE_ENUM(ArrayOrderEnum);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_NONE);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_RGB);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_RGBA);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_ARGB);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_BGR);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_BGRA);
    SDL2R_DEFINE_ENUM_VALUE(ArrayOrderEnum, ARRAYORDER_ABGR);

    SDL2R_DEFINE_ENUM(PackedLayoutEnum);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_NONE);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_332);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_4444);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_1555);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_5551);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_565);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_8888);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_2101010);
    SDL2R_DEFINE_ENUM_VALUE(PackedLayoutEnum, PACKEDLAYOUT_1010102);

    SDL2R_DEFINE_ENUM(BlendModeEnum);
    SDL2R_DEFINE_ENUM_VALUE(BlendModeEnum, BLENDMODE_NONE);
    SDL2R_DEFINE_ENUM_VALUE(BlendModeEnum, BLENDMODE_BLEND);
    SDL2R_DEFINE_ENUM_VALUE(BlendModeEnum, BLENDMODE_ADD);
    SDL2R_DEFINE_ENUM_VALUE(BlendModeEnum, BLENDMODE_MOD);

    SDL2R_DEFINE_ENUM(EndianEnum);
    SDL2R_DEFINE_ENUM_VALUE(EndianEnum, BYTEORDER);
    SDL2R_DEFINE_ENUM_VALUE(EndianEnum, BIG_ENDIAN);
    SDL2R_DEFINE_ENUM_VALUE(EndianEnum, LIL_ENDIAN);

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
