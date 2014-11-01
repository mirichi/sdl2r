#define SDL2RKEYBOARD
#include "sdl2r.h"
#include "sdl2r_window.h"
#include "sdl2r_keyboard.h"


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


void Init_sdl2r_keyboard(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_key_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_key_from_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_key_from_scancode, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_keyboard_focus, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_scancode_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_scancode_from_key, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_scancode_from_name, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, start_text_input, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, stop_text_input, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, set_text_input_rect, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_mod_state, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, set_mod_state, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_keyboard_state, 0);

    // Constants
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_UNKNOWN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_A);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_B);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_C);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_D);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_E);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_G);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_H);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_I);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_J);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_K);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_L);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_M);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_N);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_O);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_P);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_Q);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_R);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_S);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_T);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_U);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_V);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_W);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_X);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_Y);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_Z);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_1);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_2);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_3);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_4);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_5);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_6);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_7);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_8);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_9);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_0);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RETURN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_ESCAPE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_BACKSPACE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_TAB);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SPACE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_MINUS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_EQUALS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LEFTBRACKET);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RIGHTBRACKET);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_BACKSLASH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_NONUSHASH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SEMICOLON);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_APOSTROPHE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_GRAVE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_COMMA);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PERIOD);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SLASH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CAPSLOCK);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F1);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F2);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F3);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F4);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F5);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F6);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F7);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F8);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F9);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F10);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F11);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F12);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PRINTSCREEN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SCROLLLOCK);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PAUSE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INSERT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_HOME);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PAGEUP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_DELETE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_END);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PAGEDOWN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RIGHT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LEFT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_DOWN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_UP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_NUMLOCKCLEAR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_DIVIDE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MULTIPLY);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MINUS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_PLUS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_ENTER);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_1);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_2);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_3);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_4);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_5);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_6);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_7);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_8);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_9);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_0);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_PERIOD);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_NONUSBACKSLASH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_APPLICATION);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_POWER);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_EQUALS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F13);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F14);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F15);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F16);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F17);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F18);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F19);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F20);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F21);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F22);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F23);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_F24);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_EXECUTE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_HELP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_MENU);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SELECT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_STOP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AGAIN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_UNDO);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CUT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_COPY);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PASTE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_FIND);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_MUTE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_VOLUMEUP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_VOLUMEDOWN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_COMMA);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_EQUALSAS400);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL1);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL2);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL3);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL4);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL5);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL6);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL7);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL8);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_INTERNATIONAL9);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG1);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG2);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG3);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG4);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG5);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG6);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG7);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG8);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LANG9);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_ALTERASE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SYSREQ);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CANCEL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CLEAR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_PRIOR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RETURN2);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SEPARATOR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_OUT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_OPER);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CLEARAGAIN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CRSEL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_EXSEL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_00);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_000);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_THOUSANDSSEPARATOR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_DECIMALSEPARATOR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CURRENCYUNIT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CURRENCYSUBUNIT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_LEFTPAREN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_RIGHTPAREN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_LEFTBRACE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_RIGHTBRACE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_TAB);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_BACKSPACE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_A);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_B);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_C);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_D);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_E);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_F);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_XOR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_POWER);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_PERCENT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_LESS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_GREATER);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_AMPERSAND);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_DBLAMPERSAND);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_VERTICALBAR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_DBLVERTICALBAR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_COLON);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_HASH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_SPACE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_AT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_EXCLAM);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMSTORE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMRECALL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMCLEAR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMADD);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMSUBTRACT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMMULTIPLY);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_MEMDIVIDE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_PLUSMINUS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_CLEAR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_CLEARENTRY);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_BINARY);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_OCTAL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_DECIMAL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KP_HEXADECIMAL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LCTRL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LSHIFT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LALT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_LGUI);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RCTRL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RSHIFT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RALT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_RGUI);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_MODE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AUDIONEXT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AUDIOPREV);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AUDIOSTOP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AUDIOPLAY);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AUDIOMUTE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_MEDIASELECT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_WWW);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_MAIL);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_CALCULATOR);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_COMPUTER);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_SEARCH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_HOME);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_BACK);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_FORWARD);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_STOP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_REFRESH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_AC_BOOKMARKS);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_BRIGHTNESSDOWN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_BRIGHTNESSUP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_DISPLAYSWITCH);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KBDILLUMTOGGLE);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KBDILLUMDOWN);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_KBDILLUMUP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_EJECT);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_SLEEP);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_APP1);
    SDL2R_DEFINE_CONST(mSDL, SCANCODE_APP2);
    SDL2R_DEFINE_CONST(mSDL, NUM_SCANCODES);

#define SDL2R_DEFINE_CONST_K(c, t) rb_define_const(c, #t, INT2NUM(SDL##t))
    SDL2R_DEFINE_CONST_K(mSDL, K_UNKNOWN);
    SDL2R_DEFINE_CONST_K(mSDL, K_RETURN);
    SDL2R_DEFINE_CONST_K(mSDL, K_ESCAPE);
    SDL2R_DEFINE_CONST_K(mSDL, K_BACKSPACE);
    SDL2R_DEFINE_CONST_K(mSDL, K_TAB);
    SDL2R_DEFINE_CONST_K(mSDL, K_SPACE);
    SDL2R_DEFINE_CONST_K(mSDL, K_EXCLAIM);
    SDL2R_DEFINE_CONST_K(mSDL, K_QUOTEDBL);
    SDL2R_DEFINE_CONST_K(mSDL, K_HASH);
    SDL2R_DEFINE_CONST_K(mSDL, K_PERCENT);
    SDL2R_DEFINE_CONST_K(mSDL, K_DOLLAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_AMPERSAND);
    SDL2R_DEFINE_CONST_K(mSDL, K_QUOTE);
    SDL2R_DEFINE_CONST_K(mSDL, K_LEFTPAREN);
    SDL2R_DEFINE_CONST_K(mSDL, K_RIGHTPAREN);
    SDL2R_DEFINE_CONST_K(mSDL, K_ASTERISK);
    SDL2R_DEFINE_CONST_K(mSDL, K_PLUS);
    SDL2R_DEFINE_CONST_K(mSDL, K_COMMA);
    SDL2R_DEFINE_CONST_K(mSDL, K_MINUS);
    SDL2R_DEFINE_CONST_K(mSDL, K_PERIOD);
    SDL2R_DEFINE_CONST_K(mSDL, K_SLASH);
    SDL2R_DEFINE_CONST_K(mSDL, K_0);
    SDL2R_DEFINE_CONST_K(mSDL, K_1);
    SDL2R_DEFINE_CONST_K(mSDL, K_2);
    SDL2R_DEFINE_CONST_K(mSDL, K_3);
    SDL2R_DEFINE_CONST_K(mSDL, K_4);
    SDL2R_DEFINE_CONST_K(mSDL, K_5);
    SDL2R_DEFINE_CONST_K(mSDL, K_6);
    SDL2R_DEFINE_CONST_K(mSDL, K_7);
    SDL2R_DEFINE_CONST_K(mSDL, K_8);
    SDL2R_DEFINE_CONST_K(mSDL, K_9);
    SDL2R_DEFINE_CONST_K(mSDL, K_COLON);
    SDL2R_DEFINE_CONST_K(mSDL, K_SEMICOLON);
    SDL2R_DEFINE_CONST_K(mSDL, K_LESS);
    SDL2R_DEFINE_CONST_K(mSDL, K_EQUALS);
    SDL2R_DEFINE_CONST_K(mSDL, K_GREATER);
    SDL2R_DEFINE_CONST_K(mSDL, K_QUESTION);
    SDL2R_DEFINE_CONST_K(mSDL, K_AT);
    SDL2R_DEFINE_CONST_K(mSDL, K_LEFTBRACKET);
    SDL2R_DEFINE_CONST_K(mSDL, K_BACKSLASH);
    SDL2R_DEFINE_CONST_K(mSDL, K_RIGHTBRACKET);
    SDL2R_DEFINE_CONST_K(mSDL, K_CARET);
    SDL2R_DEFINE_CONST_K(mSDL, K_UNDERSCORE);
    SDL2R_DEFINE_CONST_K(mSDL, K_BACKQUOTE);
    SDL2R_DEFINE_CONST_K(mSDL, K_a);
    SDL2R_DEFINE_CONST_K(mSDL, K_b);
    SDL2R_DEFINE_CONST_K(mSDL, K_c);
    SDL2R_DEFINE_CONST_K(mSDL, K_d);
    SDL2R_DEFINE_CONST_K(mSDL, K_e);
    SDL2R_DEFINE_CONST_K(mSDL, K_f);
    SDL2R_DEFINE_CONST_K(mSDL, K_g);
    SDL2R_DEFINE_CONST_K(mSDL, K_h);
    SDL2R_DEFINE_CONST_K(mSDL, K_i);
    SDL2R_DEFINE_CONST_K(mSDL, K_j);
    SDL2R_DEFINE_CONST_K(mSDL, K_k);
    SDL2R_DEFINE_CONST_K(mSDL, K_l);
    SDL2R_DEFINE_CONST_K(mSDL, K_m);
    SDL2R_DEFINE_CONST_K(mSDL, K_n);
    SDL2R_DEFINE_CONST_K(mSDL, K_o);
    SDL2R_DEFINE_CONST_K(mSDL, K_p);
    SDL2R_DEFINE_CONST_K(mSDL, K_q);
    SDL2R_DEFINE_CONST_K(mSDL, K_r);
    SDL2R_DEFINE_CONST_K(mSDL, K_s);
    SDL2R_DEFINE_CONST_K(mSDL, K_t);
    SDL2R_DEFINE_CONST_K(mSDL, K_u);
    SDL2R_DEFINE_CONST_K(mSDL, K_v);
    SDL2R_DEFINE_CONST_K(mSDL, K_w);
    SDL2R_DEFINE_CONST_K(mSDL, K_x);
    SDL2R_DEFINE_CONST_K(mSDL, K_y);
    SDL2R_DEFINE_CONST_K(mSDL, K_z);
    SDL2R_DEFINE_CONST_K(mSDL, K_CAPSLOCK);
    SDL2R_DEFINE_CONST_K(mSDL, K_F1);
    SDL2R_DEFINE_CONST_K(mSDL, K_F2);
    SDL2R_DEFINE_CONST_K(mSDL, K_F3);
    SDL2R_DEFINE_CONST_K(mSDL, K_F4);
    SDL2R_DEFINE_CONST_K(mSDL, K_F5);
    SDL2R_DEFINE_CONST_K(mSDL, K_F6);
    SDL2R_DEFINE_CONST_K(mSDL, K_F7);
    SDL2R_DEFINE_CONST_K(mSDL, K_F8);
    SDL2R_DEFINE_CONST_K(mSDL, K_F9);
    SDL2R_DEFINE_CONST_K(mSDL, K_F10);
    SDL2R_DEFINE_CONST_K(mSDL, K_F11);
    SDL2R_DEFINE_CONST_K(mSDL, K_F12);
    SDL2R_DEFINE_CONST_K(mSDL, K_PRINTSCREEN);
    SDL2R_DEFINE_CONST_K(mSDL, K_SCROLLLOCK);
    SDL2R_DEFINE_CONST_K(mSDL, K_PAUSE);
    SDL2R_DEFINE_CONST_K(mSDL, K_INSERT);
    SDL2R_DEFINE_CONST_K(mSDL, K_HOME);
    SDL2R_DEFINE_CONST_K(mSDL, K_PAGEUP);
    SDL2R_DEFINE_CONST_K(mSDL, K_DELETE);
    SDL2R_DEFINE_CONST_K(mSDL, K_END);
    SDL2R_DEFINE_CONST_K(mSDL, K_PAGEDOWN);
    SDL2R_DEFINE_CONST_K(mSDL, K_RIGHT);
    SDL2R_DEFINE_CONST_K(mSDL, K_LEFT);
    SDL2R_DEFINE_CONST_K(mSDL, K_DOWN);
    SDL2R_DEFINE_CONST_K(mSDL, K_UP);
    SDL2R_DEFINE_CONST_K(mSDL, K_NUMLOCKCLEAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_DIVIDE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MULTIPLY);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MINUS);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_PLUS);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_ENTER);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_1);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_2);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_3);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_4);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_5);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_6);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_7);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_8);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_9);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_0);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_PERIOD);
    SDL2R_DEFINE_CONST_K(mSDL, K_APPLICATION);
    SDL2R_DEFINE_CONST_K(mSDL, K_POWER);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_EQUALS);
    SDL2R_DEFINE_CONST_K(mSDL, K_F13);
    SDL2R_DEFINE_CONST_K(mSDL, K_F14);
    SDL2R_DEFINE_CONST_K(mSDL, K_F15);
    SDL2R_DEFINE_CONST_K(mSDL, K_F16);
    SDL2R_DEFINE_CONST_K(mSDL, K_F17);
    SDL2R_DEFINE_CONST_K(mSDL, K_F18);
    SDL2R_DEFINE_CONST_K(mSDL, K_F19);
    SDL2R_DEFINE_CONST_K(mSDL, K_F20);
    SDL2R_DEFINE_CONST_K(mSDL, K_F21);
    SDL2R_DEFINE_CONST_K(mSDL, K_F22);
    SDL2R_DEFINE_CONST_K(mSDL, K_F23);
    SDL2R_DEFINE_CONST_K(mSDL, K_F24);
    SDL2R_DEFINE_CONST_K(mSDL, K_EXECUTE);
    SDL2R_DEFINE_CONST_K(mSDL, K_HELP);
    SDL2R_DEFINE_CONST_K(mSDL, K_MENU);
    SDL2R_DEFINE_CONST_K(mSDL, K_SELECT);
    SDL2R_DEFINE_CONST_K(mSDL, K_STOP);
    SDL2R_DEFINE_CONST_K(mSDL, K_AGAIN);
    SDL2R_DEFINE_CONST_K(mSDL, K_UNDO);
    SDL2R_DEFINE_CONST_K(mSDL, K_CUT);
    SDL2R_DEFINE_CONST_K(mSDL, K_COPY);
    SDL2R_DEFINE_CONST_K(mSDL, K_PASTE);
    SDL2R_DEFINE_CONST_K(mSDL, K_FIND);
    SDL2R_DEFINE_CONST_K(mSDL, K_MUTE);
    SDL2R_DEFINE_CONST_K(mSDL, K_VOLUMEUP);
    SDL2R_DEFINE_CONST_K(mSDL, K_VOLUMEDOWN);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_COMMA);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_EQUALSAS400);
    SDL2R_DEFINE_CONST_K(mSDL, K_ALTERASE);
    SDL2R_DEFINE_CONST_K(mSDL, K_SYSREQ);
    SDL2R_DEFINE_CONST_K(mSDL, K_CANCEL);
    SDL2R_DEFINE_CONST_K(mSDL, K_CLEAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_PRIOR);
    SDL2R_DEFINE_CONST_K(mSDL, K_RETURN2);
    SDL2R_DEFINE_CONST_K(mSDL, K_SEPARATOR);
    SDL2R_DEFINE_CONST_K(mSDL, K_OUT);
    SDL2R_DEFINE_CONST_K(mSDL, K_OPER);
    SDL2R_DEFINE_CONST_K(mSDL, K_CLEARAGAIN);
    SDL2R_DEFINE_CONST_K(mSDL, K_CRSEL);
    SDL2R_DEFINE_CONST_K(mSDL, K_EXSEL);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_00);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_000);
    SDL2R_DEFINE_CONST_K(mSDL, K_THOUSANDSSEPARATOR);
    SDL2R_DEFINE_CONST_K(mSDL, K_DECIMALSEPARATOR);
    SDL2R_DEFINE_CONST_K(mSDL, K_CURRENCYUNIT);
    SDL2R_DEFINE_CONST_K(mSDL, K_CURRENCYSUBUNIT);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_LEFTPAREN);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_RIGHTPAREN);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_LEFTBRACE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_RIGHTBRACE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_TAB);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_BACKSPACE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_A);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_B);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_C);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_D);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_E);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_F);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_XOR);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_POWER);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_PERCENT);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_LESS);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_GREATER);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_AMPERSAND);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_DBLAMPERSAND);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_VERTICALBAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_DBLVERTICALBAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_COLON);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_HASH);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_SPACE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_AT);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_EXCLAM);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMSTORE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMRECALL);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMCLEAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMADD);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMSUBTRACT);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMMULTIPLY);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_MEMDIVIDE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_PLUSMINUS);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_CLEAR);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_CLEARENTRY);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_BINARY);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_OCTAL);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_DECIMAL);
    SDL2R_DEFINE_CONST_K(mSDL, K_KP_HEXADECIMAL);
    SDL2R_DEFINE_CONST_K(mSDL, K_LCTRL);
    SDL2R_DEFINE_CONST_K(mSDL, K_LSHIFT);
    SDL2R_DEFINE_CONST_K(mSDL, K_LALT);
    SDL2R_DEFINE_CONST_K(mSDL, K_LGUI);
    SDL2R_DEFINE_CONST_K(mSDL, K_RCTRL);
    SDL2R_DEFINE_CONST_K(mSDL, K_RSHIFT);
    SDL2R_DEFINE_CONST_K(mSDL, K_RALT);
    SDL2R_DEFINE_CONST_K(mSDL, K_RGUI);
    SDL2R_DEFINE_CONST_K(mSDL, K_MODE);
    SDL2R_DEFINE_CONST_K(mSDL, K_AUDIONEXT);
    SDL2R_DEFINE_CONST_K(mSDL, K_AUDIOPREV);
    SDL2R_DEFINE_CONST_K(mSDL, K_AUDIOSTOP);
    SDL2R_DEFINE_CONST_K(mSDL, K_AUDIOPLAY);
    SDL2R_DEFINE_CONST_K(mSDL, K_AUDIOMUTE);
    SDL2R_DEFINE_CONST_K(mSDL, K_MEDIASELECT);
    SDL2R_DEFINE_CONST_K(mSDL, K_WWW);
    SDL2R_DEFINE_CONST_K(mSDL, K_MAIL);
    SDL2R_DEFINE_CONST_K(mSDL, K_CALCULATOR);
    SDL2R_DEFINE_CONST_K(mSDL, K_COMPUTER);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_SEARCH);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_HOME);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_BACK);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_FORWARD);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_STOP);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_REFRESH);
    SDL2R_DEFINE_CONST_K(mSDL, K_AC_BOOKMARKS);
    SDL2R_DEFINE_CONST_K(mSDL, K_BRIGHTNESSDOWN);
    SDL2R_DEFINE_CONST_K(mSDL, K_BRIGHTNESSUP);
    SDL2R_DEFINE_CONST_K(mSDL, K_DISPLAYSWITCH);
    SDL2R_DEFINE_CONST_K(mSDL, K_KBDILLUMTOGGLE);
    SDL2R_DEFINE_CONST_K(mSDL, K_KBDILLUMDOWN);
    SDL2R_DEFINE_CONST_K(mSDL, K_KBDILLUMUP);
    SDL2R_DEFINE_CONST_K(mSDL, K_EJECT);
    SDL2R_DEFINE_CONST_K(mSDL, K_SLEEP);

#define SDL2R_DEFINE_CONST_N(c, t) rb_define_const(c, #t, INT2NUM(t))
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_NONE);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_LSHIFT);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_RSHIFT);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_LCTRL);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_RCTRL);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_LALT);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_RALT);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_LGUI);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_RGUI);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_NUM);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_CAPS);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_MODE);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_RESERVED);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_CTRL);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_SHIFT);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_ALT);
    SDL2R_DEFINE_CONST_N(mSDL, KMOD_GUI);
}
