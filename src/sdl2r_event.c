#define SDL2REVENT
#include "sdl2r.h"
#include "sdl2r_event.h"

static VALUE cKeyboardEvent;
static VALUE cKeysym;
static VALUE cQuitEvent;
static VALUE cMouseMotionEvent;
static VALUE cMouseButtonEvent;
static VALUE cMouseWheelEvent;
static VALUE cJoyAxisEvent;
static VALUE cJoyBallEvent;
static VALUE cJoyButtonEvent;
static VALUE cJoyDeviceEvent;
static VALUE cJoyHatEvent;
static VALUE cWindowEvent;
static VALUE cUserEvent;
static VALUE cAppTerminating;
static VALUE cAppLowMemory;
static VALUE cAppWillEnterBackground;
static VALUE cAppDidEnterBackground;
static VALUE cAppWillEnterForeground;
static VALUE cAppDidEnterForeground;

static Sint32 g_user_event_id = 0;
static VALUE g_user_event_data = Qnil;

static VALUE sdl2r_poll_event(VALUE klass)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
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
        case SDL_MOUSEWHEEL:
            {
                VALUE ary[6];
                ary[0] = INT2NUM(event.wheel.type);
                ary[1] = INT2NUM(event.wheel.timestamp);
                ary[2] = INT2NUM(event.wheel.windowID);
                ary[3] = INT2NUM(event.wheel.which);
                ary[4] = INT2NUM(event.wheel.x);
                ary[5] = INT2NUM(event.wheel.y);
                return rb_class_new_instance(6, ary, cMouseWheelEvent);
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
        case SDL_WINDOWEVENT:
            {
                VALUE ary[6];
                ary[0] = INT2NUM(event.window.type);
                ary[1] = INT2NUM(event.window.timestamp);
                ary[2] = INT2NUM(event.window.windowID);
                ary[3] = INT2NUM(event.window.event);
                ary[4] = INT2NUM(event.window.data1);
                ary[5] = INT2NUM(event.window.data2);
                return rb_class_new_instance(6, ary, cWindowEvent);
            }
            break;
        case SDL_APP_TERMINATING:
            {
                VALUE ary[1];
                ary[0] = INT2NUM(event.type);
                return rb_class_new_instance(1, ary, cAppTerminating);
            }
            break;
        case SDL_APP_LOWMEMORY:
            {
                VALUE ary[1];
                ary[0] = INT2NUM(event.type);
                return rb_class_new_instance(1, ary, cAppLowMemory);
            }
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            {
                VALUE ary[1];
                ary[0] = INT2NUM(event.type);
                return rb_class_new_instance(1, ary, cAppWillEnterBackground);
            }
            break;
        case SDL_APP_DIDENTERBACKGROUND:
            {
                VALUE ary[1];
                ary[0] = INT2NUM(event.type);
                return rb_class_new_instance(1, ary, cAppDidEnterBackground);
            }
            break;
        case SDL_APP_WILLENTERFOREGROUND:
            {
                VALUE ary[1];
                ary[0] = INT2NUM(event.type);
                return rb_class_new_instance(1, ary, cAppWillEnterForeground);
            }
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            {
                VALUE ary[1];
                ary[0] = INT2NUM(event.type);
                return rb_class_new_instance(1, ary, cAppDidEnterForeground);
            }
            break;
        default:
            if (event.type >= SDL_USEREVENT) {
                VALUE vevent = rb_hash_lookup(g_user_event_data, INT2NUM(event.user.code));
                rb_hash_delete(g_user_event_data, INT2NUM(event.user.code));
                return vevent;
            }
            break;
        }
    }

    return Qnil;
}


static VALUE sdl2r_push_event(VALUE klass, VALUE vevent)
{
    SDL_Event ev;
    int result;

    if (NUM2UINT(rb_struct_aref(vevent, INT2FIX(0))) < SDL_USEREVENT) {
        rb_raise(eSDL2RError, "not userevent");
    }

    ev.user.type = NUM2UINT(rb_struct_aref(vevent, INT2FIX(0)));
    ev.user.timestamp = 0;
    ev.user.windowID = 0;
    ev.user.code = g_user_event_id;

    ev.user.data1 = 0;
    ev.user.data2 = 0;
    rb_hash_aset(g_user_event_data, INT2NUM(g_user_event_id), vevent);
    g_user_event_id++;

    result = SDL_PushEvent(&ev);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_register_events(VALUE klass, VALUE vnum)
{
    Uint32 result;

    result = SDL_RegisterEvents(NUM2UINT(vnum));
    if (result == (Uint32)-1) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return UINT2NUM(result);
}


static VALUE sdl2r_event_state(VALUE klass, VALUE vtype, VALUE vstate)
{
    Uint32 result;

    result = SDL_EventState(NUM2UINT(vtype), NUM2INT(vstate));
    return UINT2NUM(result);
}


static VALUE sdl2r_BUTTON(VALUE klass, VALUE vx)
{
    return INT2FIX(SDL_BUTTON(NUM2INT(vx)));
}


void Init_sdl2r_event(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, poll_event, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, push_event, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, register_events, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, event_state, 2);

    // SDL macro
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, BUTTON, 1);

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

    cMouseWheelEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "which", "x", "y", NULL);
    rb_define_const(mSDL, "MouseWheelEvent", cMouseWheelEvent);

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

    cWindowEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "event", "data1", "data2", NULL);
    rb_define_const(mSDL, "WindowEvent", cWindowEvent);

    cAppTerminating = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "AppTerminating", cAppTerminating);

    cAppLowMemory = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "AppLowMemory", cAppLowMemory);

    cAppWillEnterBackground = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "AppWillEnterBackground", cAppWillEnterBackground);

    cAppDidEnterBackground = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "AppDidEnterBackground", cAppDidEnterBackground);

    cAppWillEnterForeground = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "AppWillEnterForeground", cAppWillEnterForeground);

    cAppDidEnterForeground = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "AppDidEnterForeground", cAppDidEnterForeground);

    cUserEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "code", "data1", "data2", NULL);
    rb_define_const(mSDL, "UserEvent", cUserEvent);

    // User Event hash
    g_user_event_data = rb_hash_new();
    rb_global_variable(&g_user_event_data);

    // Constants
    SDL2R_DEFINE_CONST(mSDL, PRESSED);
    SDL2R_DEFINE_CONST(mSDL, RELEASED);
    SDL2R_DEFINE_CONST(mSDL, HAT_CENTERED);
    SDL2R_DEFINE_CONST(mSDL, HAT_UP);
    SDL2R_DEFINE_CONST(mSDL, HAT_RIGHT);
    SDL2R_DEFINE_CONST(mSDL, HAT_DOWN);
    SDL2R_DEFINE_CONST(mSDL, HAT_LEFT);
    SDL2R_DEFINE_CONST(mSDL, HAT_RIGHTUP);
    SDL2R_DEFINE_CONST(mSDL, HAT_RIGHTDOWN);
    SDL2R_DEFINE_CONST(mSDL, HAT_LEFTUP);
    SDL2R_DEFINE_CONST(mSDL, HAT_LEFTDOWN);

    SDL2R_DEFINE_CONST(mSDL, FIRSTEVENT);
    SDL2R_DEFINE_CONST(mSDL, QUIT);
    SDL2R_DEFINE_CONST(mSDL, APP_TERMINATING);
    SDL2R_DEFINE_CONST(mSDL, APP_LOWMEMORY);
    SDL2R_DEFINE_CONST(mSDL, APP_WILLENTERBACKGROUND);
    SDL2R_DEFINE_CONST(mSDL, APP_DIDENTERBACKGROUND);
    SDL2R_DEFINE_CONST(mSDL, APP_WILLENTERFOREGROUND);
    SDL2R_DEFINE_CONST(mSDL, APP_DIDENTERFOREGROUND);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT);
    SDL2R_DEFINE_CONST(mSDL, SYSWMEVENT);
    SDL2R_DEFINE_CONST(mSDL, KEYDOWN);
    SDL2R_DEFINE_CONST(mSDL, KEYUP);
    SDL2R_DEFINE_CONST(mSDL, TEXTEDITING);
    SDL2R_DEFINE_CONST(mSDL, TEXTINPUT);
    SDL2R_DEFINE_CONST(mSDL, MOUSEMOTION);
    SDL2R_DEFINE_CONST(mSDL, MOUSEBUTTONDOWN);
    SDL2R_DEFINE_CONST(mSDL, MOUSEBUTTONUP);
    SDL2R_DEFINE_CONST(mSDL, MOUSEWHEEL);
    SDL2R_DEFINE_CONST(mSDL, JOYAXISMOTION);
    SDL2R_DEFINE_CONST(mSDL, JOYBALLMOTION);
    SDL2R_DEFINE_CONST(mSDL, JOYHATMOTION);
    SDL2R_DEFINE_CONST(mSDL, JOYBUTTONDOWN);
    SDL2R_DEFINE_CONST(mSDL, JOYBUTTONUP);
    SDL2R_DEFINE_CONST(mSDL, JOYDEVICEADDED);
    SDL2R_DEFINE_CONST(mSDL, JOYDEVICEREMOVED);
    SDL2R_DEFINE_CONST(mSDL, CONTROLLERAXISMOTION);
    SDL2R_DEFINE_CONST(mSDL, CONTROLLERBUTTONDOWN);
    SDL2R_DEFINE_CONST(mSDL, CONTROLLERBUTTONUP);
    SDL2R_DEFINE_CONST(mSDL, CONTROLLERDEVICEADDED);
    SDL2R_DEFINE_CONST(mSDL, CONTROLLERDEVICEREMOVED);
    SDL2R_DEFINE_CONST(mSDL, CONTROLLERDEVICEREMAPPED);
    SDL2R_DEFINE_CONST(mSDL, FINGERDOWN);
    SDL2R_DEFINE_CONST(mSDL, FINGERUP);
    SDL2R_DEFINE_CONST(mSDL, FINGERMOTION);
    SDL2R_DEFINE_CONST(mSDL, DOLLARGESTURE);
    SDL2R_DEFINE_CONST(mSDL, DOLLARRECORD);
    SDL2R_DEFINE_CONST(mSDL, MULTIGESTURE);
    SDL2R_DEFINE_CONST(mSDL, CLIPBOARDUPDATE);
    SDL2R_DEFINE_CONST(mSDL, DROPFILE);
    SDL2R_DEFINE_CONST(mSDL, RENDER_TARGETS_RESET);
//    SDL2R_DEFINE_CONST(mSDL, SDL_RENDER_DEVICE_RESET);
    SDL2R_DEFINE_CONST(mSDL, USEREVENT);
    SDL2R_DEFINE_CONST(mSDL, LASTEVENT);

    SDL2R_DEFINE_CONST(mSDL, BUTTON_LEFT);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_MIDDLE);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_RIGHT);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_X1);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_X2);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_LMASK);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_MMASK);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_RMASK);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_X1MASK);
    SDL2R_DEFINE_CONST(mSDL, BUTTON_X2MASK);

    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_NONE);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_SHOWN);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_HIDDEN);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_EXPOSED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_MOVED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_RESIZED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_SIZE_CHANGED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_MINIMIZED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_MAXIMIZED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_RESTORED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_ENTER);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_LEAVE);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_FOCUS_GAINED);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_FOCUS_LOST);
    SDL2R_DEFINE_CONST(mSDL, WINDOWEVENT_CLOSE);
}


