#define SDL2RKEYBOARD
#include "sdl2r.h"
#include "sdl2r_window.h"
#include "sdl2r_keyboard.h"

VALUE sdl2r_EnumScancode;
VALUE sdl2r_EnumKeycode;
VALUE sdl2r_EnumKeymod;

static VALUE sdl2r_get_key_name(VALUE klass, VALUE vkeycode)
{
    const char *result;

    result = SDL_GetKeyName((SDL_Keycode)NUM2INT(vkeycode));
    return SDL2R_TO_UTF8_STRING(result);
}


static VALUE sdl2r_get_key_from_name(VALUE klass, VALUE vkeyname)
{
    SDL_Keycode result;

    result = SDL_GetKeyFromName(SDL2R_TO_UTF8_PTR(vkeyname));
    return INT2NUM(result);
}


static VALUE sdl2r_get_key_from_scancode(VALUE klass, VALUE vscancode)
{
    SDL_Keycode result;

    result = SDL_GetKeyFromScancode(NUM2INT(vscancode));
    return INT2NUM(result);
}


static VALUE sdl2r_get_keyboard_focus(VALUE klass)
{
    SDL_Window *w;

    w = SDL_GetKeyboardFocus();
    return sdl2r_window_sdl_to_ruby(w);
}


static VALUE sdl2r_get_scancode_from_key(VALUE klass, VALUE vkeycode)
{
    SDL_Scancode result;

    result = SDL_GetScancodeFromKey(NUM2INT(vkeycode));
    return INT2NUM(result);
}


static VALUE sdl2r_get_scancode_from_name(VALUE klass, VALUE vkeyname)
{
    SDL_Scancode result;

    result = SDL_GetScancodeFromName(SDL2R_TO_UTF8_PTR(vkeyname));
    return INT2NUM(result);
}


static VALUE sdl2r_get_scancode_name(VALUE klass, VALUE vscancode)
{
    const char *result;

    result = SDL_GetScancodeName((SDL_Scancode)NUM2INT(vscancode));
    return SDL2R_TO_UTF8_STRING(result);
}


static VALUE sdl2r_start_text_input(VALUE klass)
{
    SDL_StartTextInput();
    return Qnil;
}


static VALUE sdl2r_stop_text_input(VALUE klass)
{
    SDL_StopTextInput();
    return Qnil;
}


static VALUE sdl2r_set_text_input_rect(VALUE klass, VALUE vrect)
{
    SDL_Rect rect;
    SDL_Rect *pr=0;

    if (vrect != Qnil) {
        SDL2R_SET_RECT(rect, vrect);
        pr = &rect;
    }

    SDL_SetTextInputRect(pr);
    return Qnil;
}


static VALUE sdl2r_get_mod_state(VALUE klass)
{
    SDL_Keymod result;

    result = SDL_GetModState();
    return INT2NUM(result);
}


static VALUE sdl2r_set_mod_state(VALUE klass, VALUE vkeymod)
{
    SDL_SetModState(NUM2INT(vkeymod));
    return Qnil;
}


static VALUE sdl2r_get_keyboard_state(VALUE klass)
{
    int numkeys, i;
    const Uint8 *c = SDL_GetKeyboardState(&numkeys);
    VALUE vresult = rb_ary_new();

    if (numkeys) {
        for (i = 0; i < numkeys; i++) {
            rb_ary_push(vresult, UINT2NUM(c[i]));
        }
    }
    return vresult;
}


static VALUE sdl2r_has_screen_keyboard_support(VALUE klass)
{
    SDL_bool result;

    result = SDL_HasScreenKeyboardSupport();
    return SDL2R_TO_BOOL(result);
}


static VALUE sdl2r_is_screen_keyboard_shown(VALUE klass, VALUE vwindow)
{
    SDL_bool result;
    struct SDL2RWindow *win = SDL2R_GET_WINDOW_STRUCT(vwindow);

    result = SDL_IsScreenKeyboardShown(win->window);
    return SDL2R_TO_BOOL(result);
}


static VALUE sdl2r_is_text_input_active(VALUE klass)
{
    SDL_bool result;

    result = SDL_IsTextInputActive();
    return SDL2R_TO_BOOL(result);
}


void Init_sdl2r_keyboard(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(get_key_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_key_from_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_key_from_scancode, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_keyboard_focus, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(get_scancode_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_scancode_from_key, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_scancode_from_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(start_text_input, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(stop_text_input, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(set_text_input_rect, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_mod_state, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(set_mod_state, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_keyboard_state, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(has_screen_keyboard_support, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(is_screen_keyboard_shown, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(is_text_input_active, 0);

    // Enum
    SDL2R_DEFINE_ENUM(EnumScancode);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_A);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_B);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_C);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_D);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_E);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_G);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_H);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_I);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_J);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_K);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_L);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_M);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_N);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_O);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_P);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_Q);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_R);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_S);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_T);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_U);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_V);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_W);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_X);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_Y);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_Z);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_1);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_3);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_4);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_5);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_6);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_7);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_8);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_9);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_0);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RETURN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_ESCAPE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_BACKSPACE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_TAB);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SPACE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_MINUS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_EQUALS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LEFTBRACKET);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RIGHTBRACKET);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_BACKSLASH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_NONUSHASH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SEMICOLON);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_APOSTROPHE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_GRAVE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_COMMA);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PERIOD);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SLASH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CAPSLOCK);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F1);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F3);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F4);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F5);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F6);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F7);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F8);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F9);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F10);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F11);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F12);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PRINTSCREEN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SCROLLLOCK);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PAUSE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INSERT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_HOME);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PAGEUP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_DELETE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_END);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PAGEDOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RIGHT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LEFT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_DOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_UP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_NUMLOCKCLEAR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_DIVIDE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MULTIPLY);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MINUS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_PLUS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_ENTER);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_1);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_3);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_4);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_5);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_6);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_7);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_8);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_9);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_0);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_PERIOD);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_NONUSBACKSLASH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_APPLICATION);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_POWER);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_EQUALS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F13);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F14);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F15);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F16);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F17);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F18);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F19);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F20);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F21);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F22);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F23);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_F24);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_EXECUTE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_HELP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_MENU);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SELECT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_STOP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AGAIN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_UNDO);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CUT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_COPY);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PASTE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_FIND);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_MUTE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_VOLUMEUP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_VOLUMEDOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_COMMA);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_EQUALSAS400);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL1);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL3);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL4);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL5);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL6);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL7);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL8);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_INTERNATIONAL9);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG1);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG3);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG4);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG5);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG6);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG7);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG8);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LANG9);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_ALTERASE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SYSREQ);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CANCEL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CLEAR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_PRIOR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RETURN2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SEPARATOR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_OUT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_OPER);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CLEARAGAIN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CRSEL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_EXSEL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_00);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_000);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_THOUSANDSSEPARATOR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_DECIMALSEPARATOR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CURRENCYUNIT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CURRENCYSUBUNIT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_LEFTPAREN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_RIGHTPAREN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_LEFTBRACE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_RIGHTBRACE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_TAB);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_BACKSPACE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_A);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_B);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_C);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_D);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_E);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_F);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_XOR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_POWER);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_PERCENT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_LESS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_GREATER);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_AMPERSAND);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_DBLAMPERSAND);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_VERTICALBAR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_DBLVERTICALBAR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_COLON);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_HASH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_SPACE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_AT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_EXCLAM);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMSTORE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMRECALL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMCLEAR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMADD);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMSUBTRACT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMMULTIPLY);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_MEMDIVIDE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_PLUSMINUS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_CLEAR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_CLEARENTRY);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_BINARY);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_OCTAL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_DECIMAL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KP_HEXADECIMAL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LCTRL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LSHIFT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LALT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_LGUI);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RCTRL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RSHIFT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RALT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_RGUI);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_MODE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AUDIONEXT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AUDIOPREV);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AUDIOSTOP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AUDIOPLAY);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AUDIOMUTE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_MEDIASELECT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_WWW);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_MAIL);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_CALCULATOR);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_COMPUTER);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_SEARCH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_HOME);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_BACK);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_FORWARD);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_STOP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_REFRESH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_AC_BOOKMARKS);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_BRIGHTNESSDOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_BRIGHTNESSUP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_DISPLAYSWITCH);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KBDILLUMTOGGLE);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KBDILLUMDOWN);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_KBDILLUMUP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_EJECT);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_SLEEP);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_APP1);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, SCANCODE_APP2);
    SDL2R_DEFINE_ENUM_VALUE(EnumScancode, NUM_SCANCODES);

#define SDL2R_DEFINE_ENUM_VALUE_K(t, v) {rb_hash_aset(sdl2r_##t, INT2NUM(SDL##v), rb_str_new(#v, strlen(#v)));rb_define_const(mSDL, #v, INT2NUM(SDL##v));}
    SDL2R_DEFINE_ENUM(EnumKeycode);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_UNKNOWN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RETURN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_ESCAPE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_BACKSPACE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_TAB);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SPACE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_EXCLAIM);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_QUOTEDBL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_HASH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PERCENT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_DOLLAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AMPERSAND);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_QUOTE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LEFTPAREN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RIGHTPAREN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_ASTERISK);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PLUS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_COMMA);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_MINUS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PERIOD);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SLASH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_0);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_1);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_2);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_3);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_4);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_5);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_6);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_7);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_8);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_9);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_COLON);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SEMICOLON);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LESS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_EQUALS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_GREATER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_QUESTION);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LEFTBRACKET);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_BACKSLASH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RIGHTBRACKET);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CARET);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_UNDERSCORE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_BACKQUOTE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_a);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_b);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_c);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_d);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_e);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_f);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_g);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_h);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_i);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_j);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_k);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_l);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_m);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_n);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_o);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_p);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_q);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_r);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_s);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_t);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_u);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_v);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_w);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_x);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_y);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_z);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CAPSLOCK);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F1);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F2);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F3);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F4);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F5);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F6);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F7);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F8);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F9);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F10);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F11);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F12);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PRINTSCREEN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SCROLLLOCK);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PAUSE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_INSERT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_HOME);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PAGEUP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_DELETE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_END);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PAGEDOWN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RIGHT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LEFT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_DOWN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_UP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_NUMLOCKCLEAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_DIVIDE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MULTIPLY);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MINUS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_PLUS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_ENTER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_1);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_2);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_3);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_4);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_5);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_6);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_7);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_8);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_9);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_0);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_PERIOD);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_APPLICATION);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_POWER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_EQUALS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F13);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F14);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F15);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F16);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F17);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F18);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F19);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F20);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F21);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F22);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F23);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_F24);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_EXECUTE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_HELP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_MENU);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SELECT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_STOP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AGAIN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_UNDO);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CUT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_COPY);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PASTE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_FIND);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_MUTE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_VOLUMEUP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_VOLUMEDOWN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_COMMA);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_EQUALSAS400);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_ALTERASE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SYSREQ);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CANCEL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CLEAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_PRIOR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RETURN2);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SEPARATOR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_OUT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_OPER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CLEARAGAIN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CRSEL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_EXSEL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_00);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_000);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_THOUSANDSSEPARATOR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_DECIMALSEPARATOR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CURRENCYUNIT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CURRENCYSUBUNIT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_LEFTPAREN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_RIGHTPAREN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_LEFTBRACE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_RIGHTBRACE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_TAB);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_BACKSPACE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_A);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_B);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_C);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_D);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_E);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_F);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_XOR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_POWER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_PERCENT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_LESS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_GREATER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_AMPERSAND);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_DBLAMPERSAND);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_VERTICALBAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_DBLVERTICALBAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_COLON);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_HASH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_SPACE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_AT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_EXCLAM);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMSTORE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMRECALL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMCLEAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMADD);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMSUBTRACT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMMULTIPLY);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_MEMDIVIDE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_PLUSMINUS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_CLEAR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_CLEARENTRY);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_BINARY);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_OCTAL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_DECIMAL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KP_HEXADECIMAL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LCTRL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LSHIFT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LALT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_LGUI);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RCTRL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RSHIFT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RALT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_RGUI);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_MODE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AUDIONEXT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AUDIOPREV);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AUDIOSTOP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AUDIOPLAY);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AUDIOMUTE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_MEDIASELECT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_WWW);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_MAIL);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_CALCULATOR);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_COMPUTER);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_SEARCH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_HOME);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_BACK);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_FORWARD);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_STOP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_REFRESH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_AC_BOOKMARKS);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_BRIGHTNESSDOWN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_BRIGHTNESSUP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_DISPLAYSWITCH);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KBDILLUMTOGGLE);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KBDILLUMDOWN);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_KBDILLUMUP);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_EJECT);
    SDL2R_DEFINE_ENUM_VALUE_K(EnumKeycode, K_SLEEP);

    SDL2R_DEFINE_ENUM(EnumKeymod);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_NONE);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_LSHIFT);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_RSHIFT);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_LCTRL);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_RCTRL);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_LALT);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_RALT);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_LGUI);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_RGUI);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_NUM);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_CAPS);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_MODE);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_RESERVED);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_CTRL);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_SHIFT);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_ALT);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumKeymod, KMOD_GUI);
}
