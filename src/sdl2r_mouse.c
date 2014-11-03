#define SDL2RMOUSE
#include "sdl2r.h"
#include "sdl2r_mouse.h"

VALUE cCursor;

VALUE sdl2r_EnumSystemCursor;

static void sdl2r_cursor_free(void *ptr);

const rb_data_type_t sdl2r_cursor_data_type = {
    "Cursor",
    {
    0,
    sdl2r_cursor_free,
    0,
    },
};


static void sdl2r_cursor_free(void *ptr)
{
    struct SDL2RCursor *cs = ptr;

    if (cs->cursor) {
        SDL_FreeCursor(cs->cursor);
    }

    xfree(cs);
}


VALUE sdl2r_cursor_alloc(VALUE klass)
{
    struct SDL2RCursor *cs;
    VALUE vcursor = TypedData_Make_Struct(klass, struct SDL2RCursor, &sdl2r_cursor_data_type, cs);
    cs->cursor = 0;
    return vcursor;
}


static VALUE sdl2r_cursor_im_dispose(VALUE self)
{
    struct SDL2RCursor *cs = SDL2R_GET_CURSOR_STRUCT(self);

    SDL_FreeCursor(cs->cursor);
    cs->cursor = 0;

    return self;
}


static VALUE sdl2r_cursor_im_get_disposed(VALUE self)
{
    struct SDL2RCursor *cs = SDL2R_GET_STRUCT(Cursor, self);
    return cs->cursor ? Qfalse : Qtrue;
}


static VALUE sdl2r_free_cursor(VALUE klass, VALUE vcursor)
{
    struct SDL2RCursor *cs = SDL2R_GET_CURSOR_STRUCT(vcursor);

    SDL_FreeCursor(cs->cursor);
    cs->cursor = 0;

    return vcursor;
}


static VALUE sdl2r_create_system_cursor(VALUE klass, VALUE vid)
{
    VALUE vcursor = sdl2r_cursor_alloc(cCursor);
    struct SDL2RCursor *cur = SDL2R_GET_STRUCT(Cursor, vcursor);

    SDL2R_RETRY(cur->cursor = SDL_CreateSystemCursor(NUM2INT(vid)));
    if (!cur->cursor) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vcursor;
}


static VALUE sdl2r_set_cursor(VALUE klass, VALUE vcursor)
{
    struct SDL2RCursor *cur = SDL2R_GET_CURSOR_STRUCT(vcursor);

    SDL_SetCursor(cur->cursor);

    return vcursor;
}


void Init_sdl2r_mouse(void)
{
    // SDL module methods
//    SDL2R_DEFINE_SINGLETON_METHOD(capture_mouse, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(create_color_cursor, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(create_cursor, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(create_system_cursor, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(free_cursor, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_cursor, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_default_cursor, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_global_mouse_state, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_mouse_focus, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_mouse_state, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_relative_mouse_mode, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(get_relative_mouse_state, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(set_cursor, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(set_relative_mouse_mode, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(show_cursor, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(warp_mouse_global, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(warp_mouse_in_window, 0);

    // SDL::Cursor class
    cCursor = rb_define_class_under(mSDL, "Cursor", rb_cObject);
    rb_define_alloc_func(cCursor, sdl2r_cursor_alloc);
    rb_define_method(cCursor, "dispose", sdl2r_cursor_im_dispose, 0);
    rb_define_method(cCursor, "disposed?", sdl2r_cursor_im_get_disposed, 0);

    // enum
    SDL2R_DEFINE_ENUM(EnumSystemCursor);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_ARROW);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_IBEAM);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_WAIT);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_CROSSHAIR);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_WAITARROW);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_SIZENWSE);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_SIZENESW);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_SIZEWE);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_SIZENS);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_SIZEALL);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_NO);
    SDL2R_DEFINE_ENUM_VALUE(EnumSystemCursor, SYSTEM_CURSOR_HAND);

}
