#include <SDL.h>

#define SDL2REVENT
#include "sdl2r.h"
#include "sdl2r_event.h"

VALUE cKeyboardEvent;
VALUE cKeysym;
VALUE cQuitEvent;
VALUE cMouseMotionEvent;
VALUE cMouseButtonEvent;
VALUE cJoyAxisEvent;
VALUE cJoyBallEvent;
VALUE cJoyButtonEvent;
VALUE cJoyDeviceEvent;
VALUE cJoyHatEvent;

static VALUE sdl2r_poll_event(VALUE klass)
{
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            {
                VALUE ary[2];
                ary[0] = INT2NUM(event.quit.type);
                ary[1] = INT2NUM(event.quit.timestamp);
                return rb_class_new_instance(2, ary, cQuitEvent);
            }
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            {
                VALUE ary[6], keysym[4];
                keysym[0] = INT2NUM(event.key.keysym.scancode);
                keysym[1] = INT2NUM(event.key.keysym.sym);
                keysym[2] = INT2NUM(event.key.keysym.mod);
                keysym[3] = INT2NUM(event.key.keysym.unused);
                ary[0] = INT2NUM(event.key.type);
                ary[1] = INT2NUM(event.key.timestamp);
                ary[2] = INT2NUM(event.key.windowID);
                ary[3] = INT2NUM(event.key.state);
                ary[4] = INT2NUM(event.key.repeat);
                ary[5] = rb_class_new_instance(4, keysym, cKeysym);
                return rb_class_new_instance(6, ary, cKeyboardEvent);
            }
            break;
        case SDL_MOUSEMOTION:
            {
                VALUE ary[9];
                ary[0] = INT2NUM(event.motion.type);
                ary[1] = INT2NUM(event.motion.timestamp);
                ary[2] = INT2NUM(event.motion.windowID);
                ary[3] = INT2NUM(event.motion.which);
                ary[4] = INT2NUM(event.motion.state);
                ary[5] = INT2NUM(event.motion.x);
                ary[6] = INT2NUM(event.motion.y);
                ary[7] = INT2NUM(event.motion.xrel);
                ary[8] = INT2NUM(event.motion.yrel);
                return rb_class_new_instance(9, ary, cMouseMotionEvent);
            }
            break;
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            {
                VALUE ary[9];
                ary[0] = INT2NUM(event.button.type);
                ary[1] = INT2NUM(event.button.timestamp);
                ary[2] = INT2NUM(event.button.windowID);
                ary[3] = INT2NUM(event.button.which);
                ary[4] = INT2NUM(event.button.button);
                ary[5] = INT2NUM(event.button.state);
                ary[6] = INT2NUM(event.button.clicks);
                ary[7] = INT2NUM(event.button.x);
                ary[8] = INT2NUM(event.button.y);
                return rb_class_new_instance(9, ary, cMouseButtonEvent);
            }
            break;
        case SDL_JOYAXISMOTION:
            {
                VALUE ary[5];
                ary[0] = INT2NUM(event.jaxis.type);
                ary[1] = INT2NUM(event.jaxis.timestamp);
                ary[2] = INT2NUM(event.jaxis.which);
                ary[3] = INT2NUM(event.jaxis.axis);
                ary[4] = INT2NUM(event.jaxis.value);
                return rb_class_new_instance(5, ary, cJoyAxisEvent);
            }
            break;
        case SDL_JOYBALLMOTION:
            {
                VALUE ary[6];
                ary[0] = INT2NUM(event.jball.type);
                ary[1] = INT2NUM(event.jball.timestamp);
                ary[2] = INT2NUM(event.jball.which);
                ary[3] = INT2NUM(event.jball.ball);
                ary[4] = INT2NUM(event.jball.xrel);
                ary[5] = INT2NUM(event.jball.yrel);
                return rb_class_new_instance(6, ary, cJoyBallEvent);
            }
            break;
        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
            {
                VALUE ary[5];
                ary[0] = INT2NUM(event.jbutton.type);
                ary[1] = INT2NUM(event.jbutton.timestamp);
                ary[2] = INT2NUM(event.jbutton.which);
                ary[3] = INT2NUM(event.jbutton.button);
                ary[4] = INT2NUM(event.jbutton.state);
                return rb_class_new_instance(5, ary, cJoyButtonEvent);
            }
            break;
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            {
                VALUE ary[3];
                ary[0] = INT2NUM(event.jdevice.type);
                ary[1] = INT2NUM(event.jdevice.timestamp);
                ary[2] = INT2NUM(event.jdevice.which);
                return rb_class_new_instance(3, ary, cJoyDeviceEvent);
            }
            break;
        case SDL_JOYHATMOTION:
            {
                VALUE ary[5];
                ary[0] = INT2NUM(event.jhat.type);
                ary[1] = INT2NUM(event.jhat.timestamp);
                ary[2] = INT2NUM(event.jhat.which);
                ary[3] = INT2NUM(event.jhat.hat);
                ary[4] = INT2NUM(event.jhat.value);
                return rb_class_new_instance(5, ary, cJoyHatEvent);
            }
            break;
        default:
            return Qnil;
            break;
        }
    } else {
        return Qnil;
    }
}

static VALUE sdl2r_macro_button(VALUE klass, VALUE vx)
{
    return INT2FIX(SDL_BUTTON(NUM2INT(vx)));
}


void Init_sdl2r_event(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "poll_event", sdl2r_poll_event, 0);

    // SDL macro
    rb_define_singleton_method(mSDL, "BUTTON", sdl2r_macro_button, 1);

    // Event structs
    cKeyboardEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "state", "repeat", "keysym", NULL);
    rb_define_const(mSDL, "KeyboardEvent", cKeyboardEvent);

    cKeysym = rb_struct_define(NULL, "scancode", "sym", "mod", "unused", NULL);
    rb_define_const(mSDL, "Keysym", cKeysym);

    cQuitEvent = rb_struct_define(NULL, "type", "timestamp", NULL);
    rb_define_const(mSDL, "QuitEvent", cQuitEvent);

    cMouseMotionEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "which", "state", "x", "y", "xrel", "yrel", NULL);
    rb_define_const(mSDL, "MouseMotionEvent", cMouseMotionEvent);

    cMouseButtonEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "which", "button", "state", "clicks", "x", "y", NULL);
    rb_define_const(mSDL, "MouseButtonEvent", cMouseButtonEvent);

    cJoyAxisEvent = rb_struct_define(NULL, "type", "timestamp", "which", "axis", "value", NULL);
    rb_define_const(mSDL, "JoyAxisEvent", cJoyAxisEvent);

    cJoyBallEvent = rb_struct_define(NULL, "type", "timestamp", "which", "ball", "xrel", "yrel", NULL);
    rb_define_const(mSDL, "JoyBallEvent", cJoyBallEvent);

    cJoyButtonEvent = rb_struct_define(NULL, "type", "timestamp", "which", "button", "state", NULL);
    rb_define_const(mSDL, "JoyButtonEvent", cJoyButtonEvent);

    cJoyDeviceEvent = rb_struct_define(NULL, "type", "timestamp", "which", NULL);
    rb_define_const(mSDL, "JoyDevicesEvent", cJoyDeviceEvent);

    cJoyHatEvent = rb_struct_define(NULL, "type", "timestamp", "which", "hat", "value", NULL);
    rb_define_const(mSDL, "JoyHatEvent", cJoyHatEvent);

    // Constants
    rb_define_const(mSDL, "PRESSED", INT2FIX(SDL_PRESSED));
    rb_define_const(mSDL, "RELEASED", INT2FIX(SDL_RELEASED));
    rb_define_const(mSDL, "HAT_CENTERED", INT2FIX(SDL_HAT_CENTERED));
    rb_define_const(mSDL, "HAT_UP", INT2FIX(SDL_HAT_UP));
    rb_define_const(mSDL, "HAT_RIGHT", INT2FIX(SDL_HAT_RIGHT));
    rb_define_const(mSDL, "HAT_DOWN", INT2FIX(SDL_HAT_DOWN));
    rb_define_const(mSDL, "HAT_LEFT", INT2FIX(SDL_HAT_LEFT));
    rb_define_const(mSDL, "HAT_RIGHTUP", INT2FIX(SDL_HAT_RIGHTUP));
    rb_define_const(mSDL, "HAT_RIGHTDOWN", INT2FIX(SDL_HAT_RIGHTDOWN));
    rb_define_const(mSDL, "HAT_LEFTUP", INT2FIX(SDL_HAT_LEFTUP));
    rb_define_const(mSDL, "HAT_LEFTDOWN", INT2FIX(SDL_HAT_LEFTDOWN));
    rb_define_const(mSDL, "QUERY", INT2FIX(SDL_QUERY));
    rb_define_const(mSDL, "IGNORE", INT2FIX(SDL_IGNORE));
    rb_define_const(mSDL, "ENABLE", INT2FIX(SDL_ENABLE));

    rb_define_const(mSDL, "FIRSTEVENT", INT2FIX(SDL_FIRSTEVENT));
    rb_define_const(mSDL, "QUIT", INT2FIX(SDL_QUIT));
    rb_define_const(mSDL, "APP_TERMINATING", INT2FIX(SDL_APP_TERMINATING));
    rb_define_const(mSDL, "APP_LOWMEMORY", INT2FIX(SDL_APP_LOWMEMORY));
    rb_define_const(mSDL, "APP_WILLENTERBACKGROUND", INT2FIX(SDL_APP_WILLENTERBACKGROUND));
    rb_define_const(mSDL, "APP_DIDENTERBACKGROUND", INT2FIX(SDL_APP_DIDENTERBACKGROUND));
    rb_define_const(mSDL, "APP_WILLENTERFOREGROUND", INT2FIX(SDL_APP_WILLENTERFOREGROUND));
    rb_define_const(mSDL, "APP_DIDENTERFOREGROUND", INT2FIX(SDL_APP_DIDENTERFOREGROUND));
    rb_define_const(mSDL, "WINDOWEVENT", INT2FIX(SDL_WINDOWEVENT));
    rb_define_const(mSDL, "SYSWMEVENT", INT2FIX(SDL_SYSWMEVENT));
    rb_define_const(mSDL, "KEYDOWN", INT2FIX(SDL_KEYDOWN));
    rb_define_const(mSDL, "KEYUP", INT2FIX(SDL_KEYUP));
    rb_define_const(mSDL, "TEXTEDITING", INT2FIX(SDL_TEXTEDITING));
    rb_define_const(mSDL, "TEXTINPUT", INT2FIX(SDL_TEXTINPUT));
    rb_define_const(mSDL, "MOUSEMOTION", INT2FIX(SDL_MOUSEMOTION));
    rb_define_const(mSDL, "MOUSEBUTTONDOWN", INT2FIX(SDL_MOUSEBUTTONDOWN));
    rb_define_const(mSDL, "MOUSEBUTTONUP", INT2FIX(SDL_MOUSEBUTTONUP));
    rb_define_const(mSDL, "MOUSEWHEEL", INT2FIX(SDL_MOUSEWHEEL));
    rb_define_const(mSDL, "JOYAXISMOTION", INT2FIX(SDL_JOYAXISMOTION));
    rb_define_const(mSDL, "JOYBALLMOTION", INT2FIX(SDL_JOYBALLMOTION));
    rb_define_const(mSDL, "JOYHATMOTION", INT2FIX(SDL_JOYHATMOTION));
    rb_define_const(mSDL, "JOYBUTTONDOWN", INT2FIX(SDL_JOYBUTTONDOWN));
    rb_define_const(mSDL, "JOYBUTTONUP", INT2FIX(SDL_JOYBUTTONUP));
    rb_define_const(mSDL, "JOYDEVICEADDED", INT2FIX(SDL_JOYDEVICEADDED));
    rb_define_const(mSDL, "JOYDEVICEREMOVED", INT2FIX(SDL_JOYDEVICEREMOVED));
    rb_define_const(mSDL, "CONTROLLERAXISMOTION", INT2FIX(SDL_CONTROLLERAXISMOTION));
    rb_define_const(mSDL, "CONTROLLERBUTTONDOWN", INT2FIX(SDL_CONTROLLERBUTTONDOWN));
    rb_define_const(mSDL, "CONTROLLERBUTTONUP", INT2FIX(SDL_CONTROLLERBUTTONUP));
    rb_define_const(mSDL, "CONTROLLERDEVICEADDED", INT2FIX(SDL_CONTROLLERDEVICEADDED));
    rb_define_const(mSDL, "CONTROLLERDEVICEREMOVED", INT2FIX(SDL_CONTROLLERDEVICEREMOVED));
    rb_define_const(mSDL, "CONTROLLERDEVICEREMAPPED", INT2FIX(SDL_CONTROLLERDEVICEREMAPPED));
    rb_define_const(mSDL, "FINGERDOWN", INT2FIX(SDL_FINGERDOWN));
    rb_define_const(mSDL, "FINGERUP", INT2FIX(SDL_FINGERUP));
    rb_define_const(mSDL, "FINGERMOTION", INT2FIX(SDL_FINGERMOTION));
    rb_define_const(mSDL, "DOLLARGESTURE", INT2FIX(SDL_DOLLARGESTURE));
    rb_define_const(mSDL, "DOLLARRECORD", INT2FIX(SDL_DOLLARRECORD));
    rb_define_const(mSDL, "MULTIGESTURE", INT2FIX(SDL_MULTIGESTURE));
    rb_define_const(mSDL, "CLIPBOARDUPDATE", INT2FIX(SDL_CLIPBOARDUPDATE));
    rb_define_const(mSDL, "DROPFILE", INT2FIX(SDL_DROPFILE));
    rb_define_const(mSDL, "RENDER_TARGETS_RESET", INT2FIX(SDL_RENDER_TARGETS_RESET));
    rb_define_const(mSDL, "USEREVENT", INT2FIX(SDL_USEREVENT));
    rb_define_const(mSDL, "LASTEVENT", INT2FIX(SDL_LASTEVENT));

    rb_define_const(mSDL, "SCANCODE_UNKNOWN", INT2FIX(SDL_SCANCODE_UNKNOWN));
    rb_define_const(mSDL, "SCANCODE_A", INT2FIX(SDL_SCANCODE_A));
    rb_define_const(mSDL, "SCANCODE_B", INT2FIX(SDL_SCANCODE_B));
    rb_define_const(mSDL, "SCANCODE_C", INT2FIX(SDL_SCANCODE_C));
    rb_define_const(mSDL, "SCANCODE_D", INT2FIX(SDL_SCANCODE_D));
    rb_define_const(mSDL, "SCANCODE_E", INT2FIX(SDL_SCANCODE_E));
    rb_define_const(mSDL, "SCANCODE_F", INT2FIX(SDL_SCANCODE_F));
    rb_define_const(mSDL, "SCANCODE_G", INT2FIX(SDL_SCANCODE_G));
    rb_define_const(mSDL, "SCANCODE_H", INT2FIX(SDL_SCANCODE_H));
    rb_define_const(mSDL, "SCANCODE_I", INT2FIX(SDL_SCANCODE_I));
    rb_define_const(mSDL, "SCANCODE_J", INT2FIX(SDL_SCANCODE_J));
    rb_define_const(mSDL, "SCANCODE_K", INT2FIX(SDL_SCANCODE_K));
    rb_define_const(mSDL, "SCANCODE_L", INT2FIX(SDL_SCANCODE_L));
    rb_define_const(mSDL, "SCANCODE_M", INT2FIX(SDL_SCANCODE_M));
    rb_define_const(mSDL, "SCANCODE_N", INT2FIX(SDL_SCANCODE_N));
    rb_define_const(mSDL, "SCANCODE_O", INT2FIX(SDL_SCANCODE_O));
    rb_define_const(mSDL, "SCANCODE_P", INT2FIX(SDL_SCANCODE_P));
    rb_define_const(mSDL, "SCANCODE_Q", INT2FIX(SDL_SCANCODE_Q));
    rb_define_const(mSDL, "SCANCODE_R", INT2FIX(SDL_SCANCODE_R));
    rb_define_const(mSDL, "SCANCODE_S", INT2FIX(SDL_SCANCODE_S));
    rb_define_const(mSDL, "SCANCODE_T", INT2FIX(SDL_SCANCODE_T));
    rb_define_const(mSDL, "SCANCODE_U", INT2FIX(SDL_SCANCODE_U));
    rb_define_const(mSDL, "SCANCODE_V", INT2FIX(SDL_SCANCODE_V));
    rb_define_const(mSDL, "SCANCODE_W", INT2FIX(SDL_SCANCODE_W));
    rb_define_const(mSDL, "SCANCODE_X", INT2FIX(SDL_SCANCODE_X));
    rb_define_const(mSDL, "SCANCODE_Y", INT2FIX(SDL_SCANCODE_Y));
    rb_define_const(mSDL, "SCANCODE_Z", INT2FIX(SDL_SCANCODE_Z));
    rb_define_const(mSDL, "SCANCODE_1", INT2FIX(SDL_SCANCODE_1));
    rb_define_const(mSDL, "SCANCODE_2", INT2FIX(SDL_SCANCODE_2));
    rb_define_const(mSDL, "SCANCODE_3", INT2FIX(SDL_SCANCODE_3));
    rb_define_const(mSDL, "SCANCODE_4", INT2FIX(SDL_SCANCODE_4));
    rb_define_const(mSDL, "SCANCODE_5", INT2FIX(SDL_SCANCODE_5));
    rb_define_const(mSDL, "SCANCODE_6", INT2FIX(SDL_SCANCODE_6));
    rb_define_const(mSDL, "SCANCODE_7", INT2FIX(SDL_SCANCODE_7));
    rb_define_const(mSDL, "SCANCODE_8", INT2FIX(SDL_SCANCODE_8));
    rb_define_const(mSDL, "SCANCODE_9", INT2FIX(SDL_SCANCODE_9));
    rb_define_const(mSDL, "SCANCODE_0", INT2FIX(SDL_SCANCODE_0));
    rb_define_const(mSDL, "SCANCODE_RETURN", INT2FIX(SDL_SCANCODE_RETURN));
    rb_define_const(mSDL, "SCANCODE_ESCAPE", INT2FIX(SDL_SCANCODE_ESCAPE));
    rb_define_const(mSDL, "SCANCODE_BACKSPACE", INT2FIX(SDL_SCANCODE_BACKSPACE));
    rb_define_const(mSDL, "SCANCODE_TAB", INT2FIX(SDL_SCANCODE_TAB));
    rb_define_const(mSDL, "SCANCODE_SPACE", INT2FIX(SDL_SCANCODE_SPACE));
    rb_define_const(mSDL, "SCANCODE_MINUS", INT2FIX(SDL_SCANCODE_MINUS));
    rb_define_const(mSDL, "SCANCODE_EQUALS", INT2FIX(SDL_SCANCODE_EQUALS));
    rb_define_const(mSDL, "SCANCODE_LEFTBRACKET", INT2FIX(SDL_SCANCODE_LEFTBRACKET));
    rb_define_const(mSDL, "SCANCODE_RIGHTBRACKET", INT2FIX(SDL_SCANCODE_RIGHTBRACKET));
    rb_define_const(mSDL, "SCANCODE_BACKSLASH", INT2FIX(SDL_SCANCODE_BACKSLASH));
    rb_define_const(mSDL, "SCANCODE_NONUSHASH", INT2FIX(SDL_SCANCODE_NONUSHASH));
    rb_define_const(mSDL, "SCANCODE_SEMICOLON", INT2FIX(SDL_SCANCODE_SEMICOLON));
    rb_define_const(mSDL, "SCANCODE_APOSTROPHE", INT2FIX(SDL_SCANCODE_APOSTROPHE));
    rb_define_const(mSDL, "SCANCODE_GRAVE", INT2FIX(SDL_SCANCODE_GRAVE));
    rb_define_const(mSDL, "SCANCODE_COMMA", INT2FIX(SDL_SCANCODE_COMMA));
    rb_define_const(mSDL, "SCANCODE_PERIOD", INT2FIX(SDL_SCANCODE_PERIOD));
    rb_define_const(mSDL, "SCANCODE_SLASH", INT2FIX(SDL_SCANCODE_SLASH));
    rb_define_const(mSDL, "SCANCODE_CAPSLOCK", INT2FIX(SDL_SCANCODE_CAPSLOCK));
    rb_define_const(mSDL, "SCANCODE_F1", INT2FIX(SDL_SCANCODE_F1));
    rb_define_const(mSDL, "SCANCODE_F2", INT2FIX(SDL_SCANCODE_F2));
    rb_define_const(mSDL, "SCANCODE_F3", INT2FIX(SDL_SCANCODE_F3));
    rb_define_const(mSDL, "SCANCODE_F4", INT2FIX(SDL_SCANCODE_F4));
    rb_define_const(mSDL, "SCANCODE_F5", INT2FIX(SDL_SCANCODE_F5));
    rb_define_const(mSDL, "SCANCODE_F6", INT2FIX(SDL_SCANCODE_F6));
    rb_define_const(mSDL, "SCANCODE_F7", INT2FIX(SDL_SCANCODE_F7));
    rb_define_const(mSDL, "SCANCODE_F8", INT2FIX(SDL_SCANCODE_F8));
    rb_define_const(mSDL, "SCANCODE_F9", INT2FIX(SDL_SCANCODE_F9));
    rb_define_const(mSDL, "SCANCODE_F10", INT2FIX(SDL_SCANCODE_F10));
    rb_define_const(mSDL, "SCANCODE_F11", INT2FIX(SDL_SCANCODE_F11));
    rb_define_const(mSDL, "SCANCODE_F12", INT2FIX(SDL_SCANCODE_F12));
    rb_define_const(mSDL, "SCANCODE_PRINTSCREEN", INT2FIX(SDL_SCANCODE_PRINTSCREEN));
    rb_define_const(mSDL, "SCANCODE_SCROLLLOCK", INT2FIX(SDL_SCANCODE_SCROLLLOCK));
    rb_define_const(mSDL, "SCANCODE_PAUSE", INT2FIX(SDL_SCANCODE_PAUSE));
    rb_define_const(mSDL, "SCANCODE_INSERT", INT2FIX(SDL_SCANCODE_INSERT));
    rb_define_const(mSDL, "SCANCODE_HOME", INT2FIX(SDL_SCANCODE_HOME));
    rb_define_const(mSDL, "SCANCODE_PAGEUP", INT2FIX(SDL_SCANCODE_PAGEUP));
    rb_define_const(mSDL, "SCANCODE_DELETE", INT2FIX(SDL_SCANCODE_DELETE));
    rb_define_const(mSDL, "SCANCODE_END", INT2FIX(SDL_SCANCODE_END));
    rb_define_const(mSDL, "SCANCODE_PAGEDOWN", INT2FIX(SDL_SCANCODE_PAGEDOWN));
    rb_define_const(mSDL, "SCANCODE_RIGHT", INT2FIX(SDL_SCANCODE_RIGHT));
    rb_define_const(mSDL, "SCANCODE_LEFT", INT2FIX(SDL_SCANCODE_LEFT));
    rb_define_const(mSDL, "SCANCODE_DOWN", INT2FIX(SDL_SCANCODE_DOWN));
    rb_define_const(mSDL, "SCANCODE_UP", INT2FIX(SDL_SCANCODE_UP));
    rb_define_const(mSDL, "SCANCODE_NUMLOCKCLEAR", INT2FIX(SDL_SCANCODE_NUMLOCKCLEAR));
    rb_define_const(mSDL, "SCANCODE_KP_DIVIDE", INT2FIX(SDL_SCANCODE_KP_DIVIDE));
    rb_define_const(mSDL, "SCANCODE_KP_MULTIPLY", INT2FIX(SDL_SCANCODE_KP_MULTIPLY));
    rb_define_const(mSDL, "SCANCODE_KP_MINUS", INT2FIX(SDL_SCANCODE_KP_MINUS));
    rb_define_const(mSDL, "SCANCODE_KP_PLUS", INT2FIX(SDL_SCANCODE_KP_PLUS));
    rb_define_const(mSDL, "SCANCODE_KP_ENTER", INT2FIX(SDL_SCANCODE_KP_ENTER));
    rb_define_const(mSDL, "SCANCODE_KP_1", INT2FIX(SDL_SCANCODE_KP_1));
    rb_define_const(mSDL, "SCANCODE_KP_2", INT2FIX(SDL_SCANCODE_KP_2));
    rb_define_const(mSDL, "SCANCODE_KP_3", INT2FIX(SDL_SCANCODE_KP_3));
    rb_define_const(mSDL, "SCANCODE_KP_4", INT2FIX(SDL_SCANCODE_KP_4));
    rb_define_const(mSDL, "SCANCODE_KP_5", INT2FIX(SDL_SCANCODE_KP_5));
    rb_define_const(mSDL, "SCANCODE_KP_6", INT2FIX(SDL_SCANCODE_KP_6));
    rb_define_const(mSDL, "SCANCODE_KP_7", INT2FIX(SDL_SCANCODE_KP_7));
    rb_define_const(mSDL, "SCANCODE_KP_8", INT2FIX(SDL_SCANCODE_KP_8));
    rb_define_const(mSDL, "SCANCODE_KP_9", INT2FIX(SDL_SCANCODE_KP_9));
    rb_define_const(mSDL, "SCANCODE_KP_0", INT2FIX(SDL_SCANCODE_KP_0));
    rb_define_const(mSDL, "SCANCODE_KP_PERIOD", INT2FIX(SDL_SCANCODE_KP_PERIOD));
    rb_define_const(mSDL, "SCANCODE_NONUSBACKSLASH", INT2FIX(SDL_SCANCODE_NONUSBACKSLASH));
    rb_define_const(mSDL, "SCANCODE_APPLICATION", INT2FIX(SDL_SCANCODE_APPLICATION));
    rb_define_const(mSDL, "SCANCODE_POWER", INT2FIX(SDL_SCANCODE_POWER));
    rb_define_const(mSDL, "SCANCODE_KP_EQUALS", INT2FIX(SDL_SCANCODE_KP_EQUALS));
    rb_define_const(mSDL, "SCANCODE_F13", INT2FIX(SDL_SCANCODE_F13));
    rb_define_const(mSDL, "SCANCODE_F14", INT2FIX(SDL_SCANCODE_F14));
    rb_define_const(mSDL, "SCANCODE_F15", INT2FIX(SDL_SCANCODE_F15));
    rb_define_const(mSDL, "SCANCODE_F16", INT2FIX(SDL_SCANCODE_F16));
    rb_define_const(mSDL, "SCANCODE_F17", INT2FIX(SDL_SCANCODE_F17));
    rb_define_const(mSDL, "SCANCODE_F18", INT2FIX(SDL_SCANCODE_F18));
    rb_define_const(mSDL, "SCANCODE_F19", INT2FIX(SDL_SCANCODE_F19));
    rb_define_const(mSDL, "SCANCODE_F20", INT2FIX(SDL_SCANCODE_F20));
    rb_define_const(mSDL, "SCANCODE_F21", INT2FIX(SDL_SCANCODE_F21));
    rb_define_const(mSDL, "SCANCODE_F22", INT2FIX(SDL_SCANCODE_F22));
    rb_define_const(mSDL, "SCANCODE_F23", INT2FIX(SDL_SCANCODE_F23));
    rb_define_const(mSDL, "SCANCODE_F24", INT2FIX(SDL_SCANCODE_F24));
    rb_define_const(mSDL, "SCANCODE_EXECUTE", INT2FIX(SDL_SCANCODE_EXECUTE));
    rb_define_const(mSDL, "SCANCODE_HELP", INT2FIX(SDL_SCANCODE_HELP));
    rb_define_const(mSDL, "SCANCODE_MENU", INT2FIX(SDL_SCANCODE_MENU));
    rb_define_const(mSDL, "SCANCODE_SELECT", INT2FIX(SDL_SCANCODE_SELECT));
    rb_define_const(mSDL, "SCANCODE_STOP", INT2FIX(SDL_SCANCODE_STOP));
    rb_define_const(mSDL, "SCANCODE_AGAIN", INT2FIX(SDL_SCANCODE_AGAIN));
    rb_define_const(mSDL, "SCANCODE_UNDO", INT2FIX(SDL_SCANCODE_UNDO));
    rb_define_const(mSDL, "SCANCODE_CUT", INT2FIX(SDL_SCANCODE_CUT));
    rb_define_const(mSDL, "SCANCODE_COPY", INT2FIX(SDL_SCANCODE_COPY));
    rb_define_const(mSDL, "SCANCODE_PASTE", INT2FIX(SDL_SCANCODE_PASTE));
    rb_define_const(mSDL, "SCANCODE_FIND", INT2FIX(SDL_SCANCODE_FIND));
    rb_define_const(mSDL, "SCANCODE_MUTE", INT2FIX(SDL_SCANCODE_MUTE));
    rb_define_const(mSDL, "SCANCODE_VOLUMEUP", INT2FIX(SDL_SCANCODE_VOLUMEUP));
    rb_define_const(mSDL, "SCANCODE_VOLUMEDOWN", INT2FIX(SDL_SCANCODE_VOLUMEDOWN));
    rb_define_const(mSDL, "SCANCODE_KP_COMMA", INT2FIX(SDL_SCANCODE_KP_COMMA));
    rb_define_const(mSDL, "SCANCODE_KP_EQUALSAS400", INT2FIX(SDL_SCANCODE_KP_EQUALSAS400));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL1", INT2FIX(SDL_SCANCODE_INTERNATIONAL1));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL2", INT2FIX(SDL_SCANCODE_INTERNATIONAL2));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL3", INT2FIX(SDL_SCANCODE_INTERNATIONAL3));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL4", INT2FIX(SDL_SCANCODE_INTERNATIONAL4));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL5", INT2FIX(SDL_SCANCODE_INTERNATIONAL5));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL6", INT2FIX(SDL_SCANCODE_INTERNATIONAL6));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL7", INT2FIX(SDL_SCANCODE_INTERNATIONAL7));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL8", INT2FIX(SDL_SCANCODE_INTERNATIONAL8));
    rb_define_const(mSDL, "SCANCODE_INTERNATIONAL9", INT2FIX(SDL_SCANCODE_INTERNATIONAL9));
    rb_define_const(mSDL, "SCANCODE_LANG1", INT2FIX(SDL_SCANCODE_LANG1));
    rb_define_const(mSDL, "SCANCODE_LANG2", INT2FIX(SDL_SCANCODE_LANG2));
    rb_define_const(mSDL, "SCANCODE_LANG3", INT2FIX(SDL_SCANCODE_LANG3));
    rb_define_const(mSDL, "SCANCODE_LANG4", INT2FIX(SDL_SCANCODE_LANG4));
    rb_define_const(mSDL, "SCANCODE_LANG5", INT2FIX(SDL_SCANCODE_LANG5));
    rb_define_const(mSDL, "SCANCODE_LANG6", INT2FIX(SDL_SCANCODE_LANG6));
    rb_define_const(mSDL, "SCANCODE_LANG7", INT2FIX(SDL_SCANCODE_LANG7));
    rb_define_const(mSDL, "SCANCODE_LANG8", INT2FIX(SDL_SCANCODE_LANG8));
    rb_define_const(mSDL, "SCANCODE_LANG9", INT2FIX(SDL_SCANCODE_LANG9));
    rb_define_const(mSDL, "SCANCODE_ALTERASE", INT2FIX(SDL_SCANCODE_ALTERASE));
    rb_define_const(mSDL, "SCANCODE_SYSREQ", INT2FIX(SDL_SCANCODE_SYSREQ));
    rb_define_const(mSDL, "SCANCODE_CANCEL", INT2FIX(SDL_SCANCODE_CANCEL));
    rb_define_const(mSDL, "SCANCODE_CLEAR", INT2FIX(SDL_SCANCODE_CLEAR));
    rb_define_const(mSDL, "SCANCODE_PRIOR", INT2FIX(SDL_SCANCODE_PRIOR));
    rb_define_const(mSDL, "SCANCODE_RETURN2", INT2FIX(SDL_SCANCODE_RETURN2));
    rb_define_const(mSDL, "SCANCODE_SEPARATOR", INT2FIX(SDL_SCANCODE_SEPARATOR));
    rb_define_const(mSDL, "SCANCODE_OUT", INT2FIX(SDL_SCANCODE_OUT));
    rb_define_const(mSDL, "SCANCODE_OPER", INT2FIX(SDL_SCANCODE_OPER));
    rb_define_const(mSDL, "SCANCODE_CLEARAGAIN", INT2FIX(SDL_SCANCODE_CLEARAGAIN));
    rb_define_const(mSDL, "SCANCODE_CRSEL", INT2FIX(SDL_SCANCODE_CRSEL));
    rb_define_const(mSDL, "SCANCODE_EXSEL", INT2FIX(SDL_SCANCODE_EXSEL));
    rb_define_const(mSDL, "SCANCODE_KP_00", INT2FIX(SDL_SCANCODE_KP_00));
    rb_define_const(mSDL, "SCANCODE_KP_000", INT2FIX(SDL_SCANCODE_KP_000));
    rb_define_const(mSDL, "SCANCODE_THOUSANDSSEPARATOR", INT2FIX(SDL_SCANCODE_THOUSANDSSEPARATOR));
    rb_define_const(mSDL, "SCANCODE_DECIMALSEPARATOR", INT2FIX(SDL_SCANCODE_DECIMALSEPARATOR));
    rb_define_const(mSDL, "SCANCODE_CURRENCYUNIT", INT2FIX(SDL_SCANCODE_CURRENCYUNIT));
    rb_define_const(mSDL, "SCANCODE_CURRENCYSUBUNIT", INT2FIX(SDL_SCANCODE_CURRENCYSUBUNIT));
    rb_define_const(mSDL, "SCANCODE_KP_LEFTPAREN", INT2FIX(SDL_SCANCODE_KP_LEFTPAREN));
    rb_define_const(mSDL, "SCANCODE_KP_RIGHTPAREN", INT2FIX(SDL_SCANCODE_KP_RIGHTPAREN));
    rb_define_const(mSDL, "SCANCODE_KP_LEFTBRACE", INT2FIX(SDL_SCANCODE_KP_LEFTBRACE));
    rb_define_const(mSDL, "SCANCODE_KP_RIGHTBRACE", INT2FIX(SDL_SCANCODE_KP_RIGHTBRACE));
    rb_define_const(mSDL, "SCANCODE_KP_TAB", INT2FIX(SDL_SCANCODE_KP_TAB));
    rb_define_const(mSDL, "SCANCODE_KP_BACKSPACE", INT2FIX(SDL_SCANCODE_KP_BACKSPACE));
    rb_define_const(mSDL, "SCANCODE_KP_A", INT2FIX(SDL_SCANCODE_KP_A));
    rb_define_const(mSDL, "SCANCODE_KP_B", INT2FIX(SDL_SCANCODE_KP_B));
    rb_define_const(mSDL, "SCANCODE_KP_C", INT2FIX(SDL_SCANCODE_KP_C));
    rb_define_const(mSDL, "SCANCODE_KP_D", INT2FIX(SDL_SCANCODE_KP_D));
    rb_define_const(mSDL, "SCANCODE_KP_E", INT2FIX(SDL_SCANCODE_KP_E));
    rb_define_const(mSDL, "SCANCODE_KP_F", INT2FIX(SDL_SCANCODE_KP_F));
    rb_define_const(mSDL, "SCANCODE_KP_XOR", INT2FIX(SDL_SCANCODE_KP_XOR));
    rb_define_const(mSDL, "SCANCODE_KP_POWER", INT2FIX(SDL_SCANCODE_KP_POWER));
    rb_define_const(mSDL, "SCANCODE_KP_PERCENT", INT2FIX(SDL_SCANCODE_KP_PERCENT));
    rb_define_const(mSDL, "SCANCODE_KP_LESS", INT2FIX(SDL_SCANCODE_KP_LESS));
    rb_define_const(mSDL, "SCANCODE_KP_GREATER", INT2FIX(SDL_SCANCODE_KP_GREATER));
    rb_define_const(mSDL, "SCANCODE_KP_AMPERSAND", INT2FIX(SDL_SCANCODE_KP_AMPERSAND));
    rb_define_const(mSDL, "SCANCODE_KP_DBLAMPERSAND", INT2FIX(SDL_SCANCODE_KP_DBLAMPERSAND));
    rb_define_const(mSDL, "SCANCODE_KP_VERTICALBAR", INT2FIX(SDL_SCANCODE_KP_VERTICALBAR));
    rb_define_const(mSDL, "SCANCODE_KP_DBLVERTICALBAR", INT2FIX(SDL_SCANCODE_KP_DBLVERTICALBAR));
    rb_define_const(mSDL, "SCANCODE_KP_COLON", INT2FIX(SDL_SCANCODE_KP_COLON));
    rb_define_const(mSDL, "SCANCODE_KP_HASH", INT2FIX(SDL_SCANCODE_KP_HASH));
    rb_define_const(mSDL, "SCANCODE_KP_SPACE", INT2FIX(SDL_SCANCODE_KP_SPACE));
    rb_define_const(mSDL, "SCANCODE_KP_AT", INT2FIX(SDL_SCANCODE_KP_AT));
    rb_define_const(mSDL, "SCANCODE_KP_EXCLAM", INT2FIX(SDL_SCANCODE_KP_EXCLAM));
    rb_define_const(mSDL, "SCANCODE_KP_MEMSTORE", INT2FIX(SDL_SCANCODE_KP_MEMSTORE));
    rb_define_const(mSDL, "SCANCODE_KP_MEMRECALL", INT2FIX(SDL_SCANCODE_KP_MEMRECALL));
    rb_define_const(mSDL, "SCANCODE_KP_MEMCLEAR", INT2FIX(SDL_SCANCODE_KP_MEMCLEAR));
    rb_define_const(mSDL, "SCANCODE_KP_MEMADD", INT2FIX(SDL_SCANCODE_KP_MEMADD));
    rb_define_const(mSDL, "SCANCODE_KP_MEMSUBTRACT", INT2FIX(SDL_SCANCODE_KP_MEMSUBTRACT));
    rb_define_const(mSDL, "SCANCODE_KP_MEMMULTIPLY", INT2FIX(SDL_SCANCODE_KP_MEMMULTIPLY));
    rb_define_const(mSDL, "SCANCODE_KP_MEMDIVIDE", INT2FIX(SDL_SCANCODE_KP_MEMDIVIDE));
    rb_define_const(mSDL, "SCANCODE_KP_PLUSMINUS", INT2FIX(SDL_SCANCODE_KP_PLUSMINUS));
    rb_define_const(mSDL, "SCANCODE_KP_CLEAR", INT2FIX(SDL_SCANCODE_KP_CLEAR));
    rb_define_const(mSDL, "SCANCODE_KP_CLEARENTRY", INT2FIX(SDL_SCANCODE_KP_CLEARENTRY));
    rb_define_const(mSDL, "SCANCODE_KP_BINARY", INT2FIX(SDL_SCANCODE_KP_BINARY));
    rb_define_const(mSDL, "SCANCODE_KP_OCTAL", INT2FIX(SDL_SCANCODE_KP_OCTAL));
    rb_define_const(mSDL, "SCANCODE_KP_DECIMAL", INT2FIX(SDL_SCANCODE_KP_DECIMAL));
    rb_define_const(mSDL, "SCANCODE_KP_HEXADECIMAL", INT2FIX(SDL_SCANCODE_KP_HEXADECIMAL));
    rb_define_const(mSDL, "SCANCODE_LCTRL", INT2FIX(SDL_SCANCODE_LCTRL));
    rb_define_const(mSDL, "SCANCODE_LSHIFT", INT2FIX(SDL_SCANCODE_LSHIFT));
    rb_define_const(mSDL, "SCANCODE_LALT", INT2FIX(SDL_SCANCODE_LALT));
    rb_define_const(mSDL, "SCANCODE_LGUI", INT2FIX(SDL_SCANCODE_LGUI));
    rb_define_const(mSDL, "SCANCODE_RCTRL", INT2FIX(SDL_SCANCODE_RCTRL));
    rb_define_const(mSDL, "SCANCODE_RSHIFT", INT2FIX(SDL_SCANCODE_RSHIFT));
    rb_define_const(mSDL, "SCANCODE_RALT", INT2FIX(SDL_SCANCODE_RALT));
    rb_define_const(mSDL, "SCANCODE_RGUI", INT2FIX(SDL_SCANCODE_RGUI));
    rb_define_const(mSDL, "SCANCODE_MODE", INT2FIX(SDL_SCANCODE_MODE));
    rb_define_const(mSDL, "SCANCODE_AUDIONEXT", INT2FIX(SDL_SCANCODE_AUDIONEXT));
    rb_define_const(mSDL, "SCANCODE_AUDIOPREV", INT2FIX(SDL_SCANCODE_AUDIOPREV));
    rb_define_const(mSDL, "SCANCODE_AUDIOSTOP", INT2FIX(SDL_SCANCODE_AUDIOSTOP));
    rb_define_const(mSDL, "SCANCODE_AUDIOPLAY", INT2FIX(SDL_SCANCODE_AUDIOPLAY));
    rb_define_const(mSDL, "SCANCODE_AUDIOMUTE", INT2FIX(SDL_SCANCODE_AUDIOMUTE));
    rb_define_const(mSDL, "SCANCODE_MEDIASELECT", INT2FIX(SDL_SCANCODE_MEDIASELECT));
    rb_define_const(mSDL, "SCANCODE_WWW", INT2FIX(SDL_SCANCODE_WWW));
    rb_define_const(mSDL, "SCANCODE_MAIL", INT2FIX(SDL_SCANCODE_MAIL));
    rb_define_const(mSDL, "SCANCODE_CALCULATOR", INT2FIX(SDL_SCANCODE_CALCULATOR));
    rb_define_const(mSDL, "SCANCODE_COMPUTER", INT2FIX(SDL_SCANCODE_COMPUTER));
    rb_define_const(mSDL, "SCANCODE_AC_SEARCH", INT2FIX(SDL_SCANCODE_AC_SEARCH));
    rb_define_const(mSDL, "SCANCODE_AC_HOME", INT2FIX(SDL_SCANCODE_AC_HOME));
    rb_define_const(mSDL, "SCANCODE_AC_BACK", INT2FIX(SDL_SCANCODE_AC_BACK));
    rb_define_const(mSDL, "SCANCODE_AC_FORWARD", INT2FIX(SDL_SCANCODE_AC_FORWARD));
    rb_define_const(mSDL, "SCANCODE_AC_STOP", INT2FIX(SDL_SCANCODE_AC_STOP));
    rb_define_const(mSDL, "SCANCODE_AC_REFRESH", INT2FIX(SDL_SCANCODE_AC_REFRESH));
    rb_define_const(mSDL, "SCANCODE_AC_BOOKMARKS", INT2FIX(SDL_SCANCODE_AC_BOOKMARKS));
    rb_define_const(mSDL, "SCANCODE_BRIGHTNESSDOWN", INT2FIX(SDL_SCANCODE_BRIGHTNESSDOWN));
    rb_define_const(mSDL, "SCANCODE_BRIGHTNESSUP", INT2FIX(SDL_SCANCODE_BRIGHTNESSUP));
    rb_define_const(mSDL, "SCANCODE_DISPLAYSWITCH", INT2FIX(SDL_SCANCODE_DISPLAYSWITCH));
    rb_define_const(mSDL, "SCANCODE_KBDILLUMTOGGLE", INT2FIX(SDL_SCANCODE_KBDILLUMTOGGLE));
    rb_define_const(mSDL, "SCANCODE_KBDILLUMDOWN", INT2FIX(SDL_SCANCODE_KBDILLUMDOWN));
    rb_define_const(mSDL, "SCANCODE_KBDILLUMUP", INT2FIX(SDL_SCANCODE_KBDILLUMUP));
    rb_define_const(mSDL, "SCANCODE_EJECT", INT2FIX(SDL_SCANCODE_EJECT));
    rb_define_const(mSDL, "SCANCODE_SLEEP", INT2FIX(SDL_SCANCODE_SLEEP));
    rb_define_const(mSDL, "SCANCODE_APP1", INT2FIX(SDL_SCANCODE_APP1));
    rb_define_const(mSDL, "SCANCODE_APP2", INT2FIX(SDL_SCANCODE_APP2));
    rb_define_const(mSDL, "NUM_SCANCODES", INT2FIX(SDL_NUM_SCANCODES));

    rb_define_const(mSDL, "BUTTON_LEFT", INT2FIX(SDL_BUTTON_LEFT));
    rb_define_const(mSDL, "BUTTON_MIDDLE", INT2FIX(SDL_BUTTON_MIDDLE));
    rb_define_const(mSDL, "BUTTON_RIGHT", INT2FIX(SDL_BUTTON_RIGHT));
    rb_define_const(mSDL, "BUTTON_X1", INT2FIX(SDL_BUTTON_X1));
    rb_define_const(mSDL, "BUTTON_X2", INT2FIX(SDL_BUTTON_X2));
    rb_define_const(mSDL, "BUTTON_LMASK", INT2FIX(SDL_BUTTON_LMASK));
    rb_define_const(mSDL, "BUTTON_MMASK", INT2FIX(SDL_BUTTON_MMASK));
    rb_define_const(mSDL, "BUTTON_RMASK", INT2FIX(SDL_BUTTON_RMASK));
    rb_define_const(mSDL, "BUTTON_X1MASK", INT2FIX(SDL_BUTTON_X1MASK));
    rb_define_const(mSDL, "BUTTON_X2MASK", INT2FIX(SDL_BUTTON_X2MASK));

    rb_define_const(mSDL, "K_UNKNOWN", INT2FIX(SDLK_UNKNOWN));
    rb_define_const(mSDL, "K_RETURN", INT2FIX(SDLK_RETURN));
    rb_define_const(mSDL, "K_ESCAPE", INT2FIX(SDLK_ESCAPE));
    rb_define_const(mSDL, "K_BACKSPACE", INT2FIX(SDLK_BACKSPACE));
    rb_define_const(mSDL, "K_TAB", INT2FIX(SDLK_TAB));
    rb_define_const(mSDL, "K_SPACE", INT2FIX(SDLK_SPACE));
    rb_define_const(mSDL, "K_EXCLAIM", INT2FIX(SDLK_EXCLAIM));
    rb_define_const(mSDL, "K_QUOTEDBL", INT2FIX(SDLK_QUOTEDBL));
    rb_define_const(mSDL, "K_HASH", INT2FIX(SDLK_HASH));
    rb_define_const(mSDL, "K_PERCENT", INT2FIX(SDLK_PERCENT));
    rb_define_const(mSDL, "K_DOLLAR", INT2FIX(SDLK_DOLLAR));
    rb_define_const(mSDL, "K_AMPERSAND", INT2FIX(SDLK_AMPERSAND));
    rb_define_const(mSDL, "K_QUOTE", INT2FIX(SDLK_QUOTE));
    rb_define_const(mSDL, "K_LEFTPAREN", INT2FIX(SDLK_LEFTPAREN));
    rb_define_const(mSDL, "K_RIGHTPAREN", INT2FIX(SDLK_RIGHTPAREN));
    rb_define_const(mSDL, "K_ASTERISK", INT2FIX(SDLK_ASTERISK));
    rb_define_const(mSDL, "K_PLUS", INT2FIX(SDLK_PLUS));
    rb_define_const(mSDL, "K_COMMA", INT2FIX(SDLK_COMMA));
    rb_define_const(mSDL, "K_MINUS", INT2FIX(SDLK_MINUS));
    rb_define_const(mSDL, "K_PERIOD", INT2FIX(SDLK_PERIOD));
    rb_define_const(mSDL, "K_SLASH", INT2FIX(SDLK_SLASH));
    rb_define_const(mSDL, "K_0", INT2FIX(SDLK_0));
    rb_define_const(mSDL, "K_1", INT2FIX(SDLK_1));
    rb_define_const(mSDL, "K_2", INT2FIX(SDLK_2));
    rb_define_const(mSDL, "K_3", INT2FIX(SDLK_3));
    rb_define_const(mSDL, "K_4", INT2FIX(SDLK_4));
    rb_define_const(mSDL, "K_5", INT2FIX(SDLK_5));
    rb_define_const(mSDL, "K_6", INT2FIX(SDLK_6));
    rb_define_const(mSDL, "K_7", INT2FIX(SDLK_7));
    rb_define_const(mSDL, "K_8", INT2FIX(SDLK_8));
    rb_define_const(mSDL, "K_9", INT2FIX(SDLK_9));
    rb_define_const(mSDL, "K_COLON", INT2FIX(SDLK_COLON));
    rb_define_const(mSDL, "K_SEMICOLON", INT2FIX(SDLK_SEMICOLON));
    rb_define_const(mSDL, "K_LESS", INT2FIX(SDLK_LESS));
    rb_define_const(mSDL, "K_EQUALS", INT2FIX(SDLK_EQUALS));
    rb_define_const(mSDL, "K_GREATER", INT2FIX(SDLK_GREATER));
    rb_define_const(mSDL, "K_QUESTION", INT2FIX(SDLK_QUESTION));
    rb_define_const(mSDL, "K_AT", INT2FIX(SDLK_AT));
    rb_define_const(mSDL, "K_LEFTBRACKET", INT2FIX(SDLK_LEFTBRACKET));
    rb_define_const(mSDL, "K_BACKSLASH", INT2FIX(SDLK_BACKSLASH));
    rb_define_const(mSDL, "K_RIGHTBRACKET", INT2FIX(SDLK_RIGHTBRACKET));
    rb_define_const(mSDL, "K_CARET", INT2FIX(SDLK_CARET));
    rb_define_const(mSDL, "K_UNDERSCORE", INT2FIX(SDLK_UNDERSCORE));
    rb_define_const(mSDL, "K_BACKQUOTE", INT2FIX(SDLK_BACKQUOTE));
    rb_define_const(mSDL, "K_a", INT2FIX(SDLK_a));
    rb_define_const(mSDL, "K_b", INT2FIX(SDLK_b));
    rb_define_const(mSDL, "K_c", INT2FIX(SDLK_c));
    rb_define_const(mSDL, "K_d", INT2FIX(SDLK_d));
    rb_define_const(mSDL, "K_e", INT2FIX(SDLK_e));
    rb_define_const(mSDL, "K_f", INT2FIX(SDLK_f));
    rb_define_const(mSDL, "K_g", INT2FIX(SDLK_g));
    rb_define_const(mSDL, "K_h", INT2FIX(SDLK_h));
    rb_define_const(mSDL, "K_i", INT2FIX(SDLK_i));
    rb_define_const(mSDL, "K_j", INT2FIX(SDLK_j));
    rb_define_const(mSDL, "K_k", INT2FIX(SDLK_k));
    rb_define_const(mSDL, "K_l", INT2FIX(SDLK_l));
    rb_define_const(mSDL, "K_m", INT2FIX(SDLK_m));
    rb_define_const(mSDL, "K_n", INT2FIX(SDLK_n));
    rb_define_const(mSDL, "K_o", INT2FIX(SDLK_o));
    rb_define_const(mSDL, "K_p", INT2FIX(SDLK_p));
    rb_define_const(mSDL, "K_q", INT2FIX(SDLK_q));
    rb_define_const(mSDL, "K_r", INT2FIX(SDLK_r));
    rb_define_const(mSDL, "K_s", INT2FIX(SDLK_s));
    rb_define_const(mSDL, "K_t", INT2FIX(SDLK_t));
    rb_define_const(mSDL, "K_u", INT2FIX(SDLK_u));
    rb_define_const(mSDL, "K_v", INT2FIX(SDLK_v));
    rb_define_const(mSDL, "K_w", INT2FIX(SDLK_w));
    rb_define_const(mSDL, "K_x", INT2FIX(SDLK_x));
    rb_define_const(mSDL, "K_y", INT2FIX(SDLK_y));
    rb_define_const(mSDL, "K_z", INT2FIX(SDLK_z));
    rb_define_const(mSDL, "K_CAPSLOCK", INT2FIX(SDLK_CAPSLOCK));
    rb_define_const(mSDL, "K_F1", INT2FIX(SDLK_F1));
    rb_define_const(mSDL, "K_F2", INT2FIX(SDLK_F2));
    rb_define_const(mSDL, "K_F3", INT2FIX(SDLK_F3));
    rb_define_const(mSDL, "K_F4", INT2FIX(SDLK_F4));
    rb_define_const(mSDL, "K_F5", INT2FIX(SDLK_F5));
    rb_define_const(mSDL, "K_F6", INT2FIX(SDLK_F6));
    rb_define_const(mSDL, "K_F7", INT2FIX(SDLK_F7));
    rb_define_const(mSDL, "K_F8", INT2FIX(SDLK_F8));
    rb_define_const(mSDL, "K_F9", INT2FIX(SDLK_F9));
    rb_define_const(mSDL, "K_F10", INT2FIX(SDLK_F10));
    rb_define_const(mSDL, "K_F11", INT2FIX(SDLK_F11));
    rb_define_const(mSDL, "K_F12", INT2FIX(SDLK_F12));
    rb_define_const(mSDL, "K_PRINTSCREEN", INT2FIX(SDLK_PRINTSCREEN));
    rb_define_const(mSDL, "K_SCROLLLOCK", INT2FIX(SDLK_SCROLLLOCK));
    rb_define_const(mSDL, "K_PAUSE", INT2FIX(SDLK_PAUSE));
    rb_define_const(mSDL, "K_INSERT", INT2FIX(SDLK_INSERT));
    rb_define_const(mSDL, "K_HOME", INT2FIX(SDLK_HOME));
    rb_define_const(mSDL, "K_PAGEUP", INT2FIX(SDLK_PAGEUP));
    rb_define_const(mSDL, "K_DELETE", INT2FIX(SDLK_DELETE));
    rb_define_const(mSDL, "K_END", INT2FIX(SDLK_END));
    rb_define_const(mSDL, "K_PAGEDOWN", INT2FIX(SDLK_PAGEDOWN));
    rb_define_const(mSDL, "K_RIGHT", INT2FIX(SDLK_RIGHT));
    rb_define_const(mSDL, "K_LEFT", INT2FIX(SDLK_LEFT));
    rb_define_const(mSDL, "K_DOWN", INT2FIX(SDLK_DOWN));
    rb_define_const(mSDL, "K_UP", INT2FIX(SDLK_UP));
    rb_define_const(mSDL, "K_NUMLOCKCLEAR", INT2FIX(SDLK_NUMLOCKCLEAR));
    rb_define_const(mSDL, "K_KP_DIVIDE", INT2FIX(SDLK_KP_DIVIDE));
    rb_define_const(mSDL, "K_KP_MULTIPLY", INT2FIX(SDLK_KP_MULTIPLY));
    rb_define_const(mSDL, "K_KP_MINUS", INT2FIX(SDLK_KP_MINUS));
    rb_define_const(mSDL, "K_KP_PLUS", INT2FIX(SDLK_KP_PLUS));
    rb_define_const(mSDL, "K_KP_ENTER", INT2FIX(SDLK_KP_ENTER));
    rb_define_const(mSDL, "K_KP_1", INT2FIX(SDLK_KP_1));
    rb_define_const(mSDL, "K_KP_2", INT2FIX(SDLK_KP_2));
    rb_define_const(mSDL, "K_KP_3", INT2FIX(SDLK_KP_3));
    rb_define_const(mSDL, "K_KP_4", INT2FIX(SDLK_KP_4));
    rb_define_const(mSDL, "K_KP_5", INT2FIX(SDLK_KP_5));
    rb_define_const(mSDL, "K_KP_6", INT2FIX(SDLK_KP_6));
    rb_define_const(mSDL, "K_KP_7", INT2FIX(SDLK_KP_7));
    rb_define_const(mSDL, "K_KP_8", INT2FIX(SDLK_KP_8));
    rb_define_const(mSDL, "K_KP_9", INT2FIX(SDLK_KP_9));
    rb_define_const(mSDL, "K_KP_0", INT2FIX(SDLK_KP_0));
    rb_define_const(mSDL, "K_KP_PERIOD", INT2FIX(SDLK_KP_PERIOD));
    rb_define_const(mSDL, "K_APPLICATION", INT2FIX(SDLK_APPLICATION));
    rb_define_const(mSDL, "K_POWER", INT2FIX(SDLK_POWER));
    rb_define_const(mSDL, "K_KP_EQUALS", INT2FIX(SDLK_KP_EQUALS));
    rb_define_const(mSDL, "K_F13", INT2FIX(SDLK_F13));
    rb_define_const(mSDL, "K_F14", INT2FIX(SDLK_F14));
    rb_define_const(mSDL, "K_F15", INT2FIX(SDLK_F15));
    rb_define_const(mSDL, "K_F16", INT2FIX(SDLK_F16));
    rb_define_const(mSDL, "K_F17", INT2FIX(SDLK_F17));
    rb_define_const(mSDL, "K_F18", INT2FIX(SDLK_F18));
    rb_define_const(mSDL, "K_F19", INT2FIX(SDLK_F19));
    rb_define_const(mSDL, "K_F20", INT2FIX(SDLK_F20));
    rb_define_const(mSDL, "K_F21", INT2FIX(SDLK_F21));
    rb_define_const(mSDL, "K_F22", INT2FIX(SDLK_F22));
    rb_define_const(mSDL, "K_F23", INT2FIX(SDLK_F23));
    rb_define_const(mSDL, "K_F24", INT2FIX(SDLK_F24));
    rb_define_const(mSDL, "K_EXECUTE", INT2FIX(SDLK_EXECUTE));
    rb_define_const(mSDL, "K_HELP", INT2FIX(SDLK_HELP));
    rb_define_const(mSDL, "K_MENU", INT2FIX(SDLK_MENU));
    rb_define_const(mSDL, "K_SELECT", INT2FIX(SDLK_SELECT));
    rb_define_const(mSDL, "K_STOP", INT2FIX(SDLK_STOP));
    rb_define_const(mSDL, "K_AGAIN", INT2FIX(SDLK_AGAIN));
    rb_define_const(mSDL, "K_UNDO", INT2FIX(SDLK_UNDO));
    rb_define_const(mSDL, "K_CUT", INT2FIX(SDLK_CUT));
    rb_define_const(mSDL, "K_COPY", INT2FIX(SDLK_COPY));
    rb_define_const(mSDL, "K_PASTE", INT2FIX(SDLK_PASTE));
    rb_define_const(mSDL, "K_FIND", INT2FIX(SDLK_FIND));
    rb_define_const(mSDL, "K_MUTE", INT2FIX(SDLK_MUTE));
    rb_define_const(mSDL, "K_VOLUMEUP", INT2FIX(SDLK_VOLUMEUP));
    rb_define_const(mSDL, "K_VOLUMEDOWN", INT2FIX(SDLK_VOLUMEDOWN));
    rb_define_const(mSDL, "K_KP_COMMA", INT2FIX(SDLK_KP_COMMA));
    rb_define_const(mSDL, "K_KP_EQUALSAS400", INT2FIX(SDLK_KP_EQUALSAS400));
    rb_define_const(mSDL, "K_ALTERASE", INT2FIX(SDLK_ALTERASE));
    rb_define_const(mSDL, "K_SYSREQ", INT2FIX(SDLK_SYSREQ));
    rb_define_const(mSDL, "K_CANCEL", INT2FIX(SDLK_CANCEL));
    rb_define_const(mSDL, "K_CLEAR", INT2FIX(SDLK_CLEAR));
    rb_define_const(mSDL, "K_PRIOR", INT2FIX(SDLK_PRIOR));
    rb_define_const(mSDL, "K_RETURN2", INT2FIX(SDLK_RETURN2));
    rb_define_const(mSDL, "K_SEPARATOR", INT2FIX(SDLK_SEPARATOR));
    rb_define_const(mSDL, "K_OUT", INT2FIX(SDLK_OUT));
    rb_define_const(mSDL, "K_OPER", INT2FIX(SDLK_OPER));
    rb_define_const(mSDL, "K_CLEARAGAIN", INT2FIX(SDLK_CLEARAGAIN));
    rb_define_const(mSDL, "K_CRSEL", INT2FIX(SDLK_CRSEL));
    rb_define_const(mSDL, "K_EXSEL", INT2FIX(SDLK_EXSEL));
    rb_define_const(mSDL, "K_KP_00", INT2FIX(SDLK_KP_00));
    rb_define_const(mSDL, "K_KP_000", INT2FIX(SDLK_KP_000));
    rb_define_const(mSDL, "K_THOUSANDSSEPARATOR", INT2FIX(SDLK_THOUSANDSSEPARATOR));
    rb_define_const(mSDL, "K_DECIMALSEPARATOR", INT2FIX(SDLK_DECIMALSEPARATOR));
    rb_define_const(mSDL, "K_CURRENCYUNIT", INT2FIX(SDLK_CURRENCYUNIT));
    rb_define_const(mSDL, "K_CURRENCYSUBUNIT", INT2FIX(SDLK_CURRENCYSUBUNIT));
    rb_define_const(mSDL, "K_KP_LEFTPAREN", INT2FIX(SDLK_KP_LEFTPAREN));
    rb_define_const(mSDL, "K_KP_RIGHTPAREN", INT2FIX(SDLK_KP_RIGHTPAREN));
    rb_define_const(mSDL, "K_KP_LEFTBRACE", INT2FIX(SDLK_KP_LEFTBRACE));
    rb_define_const(mSDL, "K_KP_RIGHTBRACE", INT2FIX(SDLK_KP_RIGHTBRACE));
    rb_define_const(mSDL, "K_KP_TAB", INT2FIX(SDLK_KP_TAB));
    rb_define_const(mSDL, "K_KP_BACKSPACE", INT2FIX(SDLK_KP_BACKSPACE));
    rb_define_const(mSDL, "K_KP_A", INT2FIX(SDLK_KP_A));
    rb_define_const(mSDL, "K_KP_B", INT2FIX(SDLK_KP_B));
    rb_define_const(mSDL, "K_KP_C", INT2FIX(SDLK_KP_C));
    rb_define_const(mSDL, "K_KP_D", INT2FIX(SDLK_KP_D));
    rb_define_const(mSDL, "K_KP_E", INT2FIX(SDLK_KP_E));
    rb_define_const(mSDL, "K_KP_F", INT2FIX(SDLK_KP_F));
    rb_define_const(mSDL, "K_KP_XOR", INT2FIX(SDLK_KP_XOR));
    rb_define_const(mSDL, "K_KP_POWER", INT2FIX(SDLK_KP_POWER));
    rb_define_const(mSDL, "K_KP_PERCENT", INT2FIX(SDLK_KP_PERCENT));
    rb_define_const(mSDL, "K_KP_LESS", INT2FIX(SDLK_KP_LESS));
    rb_define_const(mSDL, "K_KP_GREATER", INT2FIX(SDLK_KP_GREATER));
    rb_define_const(mSDL, "K_KP_AMPERSAND", INT2FIX(SDLK_KP_AMPERSAND));
    rb_define_const(mSDL, "K_KP_DBLAMPERSAND", INT2FIX(SDLK_KP_DBLAMPERSAND));
    rb_define_const(mSDL, "K_KP_VERTICALBAR", INT2FIX(SDLK_KP_VERTICALBAR));
    rb_define_const(mSDL, "K_KP_DBLVERTICALBAR", INT2FIX(SDLK_KP_DBLVERTICALBAR));
    rb_define_const(mSDL, "K_KP_COLON", INT2FIX(SDLK_KP_COLON));
    rb_define_const(mSDL, "K_KP_HASH", INT2FIX(SDLK_KP_HASH));
    rb_define_const(mSDL, "K_KP_SPACE", INT2FIX(SDLK_KP_SPACE));
    rb_define_const(mSDL, "K_KP_AT", INT2FIX(SDLK_KP_AT));
    rb_define_const(mSDL, "K_KP_EXCLAM", INT2FIX(SDLK_KP_EXCLAM));
    rb_define_const(mSDL, "K_KP_MEMSTORE", INT2FIX(SDLK_KP_MEMSTORE));
    rb_define_const(mSDL, "K_KP_MEMRECALL", INT2FIX(SDLK_KP_MEMRECALL));
    rb_define_const(mSDL, "K_KP_MEMCLEAR", INT2FIX(SDLK_KP_MEMCLEAR));
    rb_define_const(mSDL, "K_KP_MEMADD", INT2FIX(SDLK_KP_MEMADD));
    rb_define_const(mSDL, "K_KP_MEMSUBTRACT", INT2FIX(SDLK_KP_MEMSUBTRACT));
    rb_define_const(mSDL, "K_KP_MEMMULTIPLY", INT2FIX(SDLK_KP_MEMMULTIPLY));
    rb_define_const(mSDL, "K_KP_MEMDIVIDE", INT2FIX(SDLK_KP_MEMDIVIDE));
    rb_define_const(mSDL, "K_KP_PLUSMINUS", INT2FIX(SDLK_KP_PLUSMINUS));
    rb_define_const(mSDL, "K_KP_CLEAR", INT2FIX(SDLK_KP_CLEAR));
    rb_define_const(mSDL, "K_KP_CLEARENTRY", INT2FIX(SDLK_KP_CLEARENTRY));
    rb_define_const(mSDL, "K_KP_BINARY", INT2FIX(SDLK_KP_BINARY));
    rb_define_const(mSDL, "K_KP_OCTAL", INT2FIX(SDLK_KP_OCTAL));
    rb_define_const(mSDL, "K_KP_DECIMAL", INT2FIX(SDLK_KP_DECIMAL));
    rb_define_const(mSDL, "K_KP_HEXADECIMAL", INT2FIX(SDLK_KP_HEXADECIMAL));
    rb_define_const(mSDL, "K_LCTRL", INT2FIX(SDLK_LCTRL));
    rb_define_const(mSDL, "K_LSHIFT", INT2FIX(SDLK_LSHIFT));
    rb_define_const(mSDL, "K_LALT", INT2FIX(SDLK_LALT));
    rb_define_const(mSDL, "K_LGUI", INT2FIX(SDLK_LGUI));
    rb_define_const(mSDL, "K_RCTRL", INT2FIX(SDLK_RCTRL));
    rb_define_const(mSDL, "K_RSHIFT", INT2FIX(SDLK_RSHIFT));
    rb_define_const(mSDL, "K_RALT", INT2FIX(SDLK_RALT));
    rb_define_const(mSDL, "K_RGUI", INT2FIX(SDLK_RGUI));
    rb_define_const(mSDL, "K_MODE", INT2FIX(SDLK_MODE));
    rb_define_const(mSDL, "K_AUDIONEXT", INT2FIX(SDLK_AUDIONEXT));
    rb_define_const(mSDL, "K_AUDIOPREV", INT2FIX(SDLK_AUDIOPREV));
    rb_define_const(mSDL, "K_AUDIOSTOP", INT2FIX(SDLK_AUDIOSTOP));
    rb_define_const(mSDL, "K_AUDIOPLAY", INT2FIX(SDLK_AUDIOPLAY));
    rb_define_const(mSDL, "K_AUDIOMUTE", INT2FIX(SDLK_AUDIOMUTE));
    rb_define_const(mSDL, "K_MEDIASELECT", INT2FIX(SDLK_MEDIASELECT));
    rb_define_const(mSDL, "K_WWW", INT2FIX(SDLK_WWW));
    rb_define_const(mSDL, "K_MAIL", INT2FIX(SDLK_MAIL));
    rb_define_const(mSDL, "K_CALCULATOR", INT2FIX(SDLK_CALCULATOR));
    rb_define_const(mSDL, "K_COMPUTER", INT2FIX(SDLK_COMPUTER));
    rb_define_const(mSDL, "K_AC_SEARCH", INT2FIX(SDLK_AC_SEARCH));
    rb_define_const(mSDL, "K_AC_HOME", INT2FIX(SDLK_AC_HOME));
    rb_define_const(mSDL, "K_AC_BACK", INT2FIX(SDLK_AC_BACK));
    rb_define_const(mSDL, "K_AC_FORWARD", INT2FIX(SDLK_AC_FORWARD));
    rb_define_const(mSDL, "K_AC_STOP", INT2FIX(SDLK_AC_STOP));
    rb_define_const(mSDL, "K_AC_REFRESH", INT2FIX(SDLK_AC_REFRESH));
    rb_define_const(mSDL, "K_AC_BOOKMARKS", INT2FIX(SDLK_AC_BOOKMARKS));
    rb_define_const(mSDL, "K_BRIGHTNESSDOWN", INT2FIX(SDLK_BRIGHTNESSDOWN));
    rb_define_const(mSDL, "K_BRIGHTNESSUP", INT2FIX(SDLK_BRIGHTNESSUP));
    rb_define_const(mSDL, "K_DISPLAYSWITCH", INT2FIX(SDLK_DISPLAYSWITCH));
    rb_define_const(mSDL, "K_KBDILLUMTOGGLE", INT2FIX(SDLK_KBDILLUMTOGGLE));
    rb_define_const(mSDL, "K_KBDILLUMDOWN", INT2FIX(SDLK_KBDILLUMDOWN));
    rb_define_const(mSDL, "K_KBDILLUMUP", INT2FIX(SDLK_KBDILLUMUP));
    rb_define_const(mSDL, "K_EJECT", INT2FIX(SDLK_EJECT));
    rb_define_const(mSDL, "K_SLEEP", INT2FIX(SDLK_SLEEP));

}


