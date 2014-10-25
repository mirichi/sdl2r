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
}


