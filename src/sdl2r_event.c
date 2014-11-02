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
static VALUE cDropEvent;
static VALUE cTextInputEvent;
static VALUE cTextEditingEvent;
static VALUE cAppTerminating;
static VALUE cAppLowMemory;
static VALUE cAppWillEnterBackground;
static VALUE cAppDidEnterBackground;
static VALUE cAppWillEnterForeground;
static VALUE cAppDidEnterForeground;
static VALUE cUserEvent;
static VALUE cControllerAxisEvent;
static VALUE cControllerButtonEvent;
static VALUE cControllerDeviceEvent;
static VALUE cDollarGestureEvent;
static VALUE cMultiGestureEvent;
static VALUE cSysWMEvent;
static VALUE cTouchFingerEvent;
static VALUE cClipboardUpdateEvent;

static Sint32 g_user_event_id = 0;
static VALUE g_user_event_data = Qnil;

static VALUE sdl2r_make_event(SDL_Event *event)
{
    switch(event->type) {
    case SDL_QUIT:
        {
            VALUE ary[2];
            ary[0] = UINT2NUM(event->quit.type);
            ary[1] = UINT2NUM(event->quit.timestamp);
            return rb_class_new_instance(2, ary, cQuitEvent);
        }
        break;
    case SDL_KEYUP:
    case SDL_KEYDOWN:
        {
            VALUE ary[6], keysym[4];
            keysym[0] = INT2NUM(event->key.keysym.scancode);
            keysym[1] = INT2NUM(event->key.keysym.sym);
            keysym[2] = UINT2NUM(event->key.keysym.mod);
            keysym[3] = UINT2NUM(event->key.keysym.unused);
            ary[0] = UINT2NUM(event->key.type);
            ary[1] = UINT2NUM(event->key.timestamp);
            ary[2] = UINT2NUM(event->key.windowID);
            ary[3] = UINT2NUM(event->key.state);
            ary[4] = UINT2NUM(event->key.repeat);
            ary[5] = rb_class_new_instance(4, keysym, cKeysym);
            return rb_class_new_instance(6, ary, cKeyboardEvent);
        }
        break;
    case SDL_MOUSEMOTION:
        {
            VALUE ary[9];
            ary[0] = UINT2NUM(event->motion.type);
            ary[1] = UINT2NUM(event->motion.timestamp);
            ary[2] = UINT2NUM(event->motion.windowID);
            ary[3] = UINT2NUM(event->motion.which);
            ary[4] = UINT2NUM(event->motion.state);
            ary[5] = INT2NUM(event->motion.x);
            ary[6] = INT2NUM(event->motion.y);
            ary[7] = INT2NUM(event->motion.xrel);
            ary[8] = INT2NUM(event->motion.yrel);
            return rb_class_new_instance(9, ary, cMouseMotionEvent);
        }
        break;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
        {
            VALUE ary[9];
            ary[0] = UINT2NUM(event->button.type);
            ary[1] = UINT2NUM(event->button.timestamp);
            ary[2] = UINT2NUM(event->button.windowID);
            ary[3] = UINT2NUM(event->button.which);
            ary[4] = UINT2NUM(event->button.button);
            ary[5] = UINT2NUM(event->button.state);
            ary[6] = UINT2NUM(event->button.clicks);
            ary[7] = INT2NUM(event->button.x);
            ary[8] = INT2NUM(event->button.y);
            return rb_class_new_instance(9, ary, cMouseButtonEvent);
        }
        break;
    case SDL_MOUSEWHEEL:
        {
            VALUE ary[6];
            ary[0] = UINT2NUM(event->wheel.type);
            ary[1] = UINT2NUM(event->wheel.timestamp);
            ary[2] = UINT2NUM(event->wheel.windowID);
            ary[3] = UINT2NUM(event->wheel.which);
            ary[4] = INT2NUM(event->wheel.x);
            ary[5] = INT2NUM(event->wheel.y);
            return rb_class_new_instance(6, ary, cMouseWheelEvent);
        }
        break;
    case SDL_JOYAXISMOTION:
        {
            VALUE ary[5];
            ary[0] = UINT2NUM(event->jaxis.type);
            ary[1] = UINT2NUM(event->jaxis.timestamp);
            ary[2] = INT2NUM(event->jaxis.which);
            ary[3] = UINT2NUM(event->jaxis.axis);
            ary[4] = INT2NUM(event->jaxis.value);
            return rb_class_new_instance(5, ary, cJoyAxisEvent);
        }
        break;
    case SDL_JOYBALLMOTION:
        {
            VALUE ary[6];
            ary[0] = UINT2NUM(event->jball.type);
            ary[1] = UINT2NUM(event->jball.timestamp);
            ary[2] = INT2NUM(event->jball.which);
            ary[3] = UINT2NUM(event->jball.ball);
            ary[4] = INT2NUM(event->jball.xrel);
            ary[5] = INT2NUM(event->jball.yrel);
            return rb_class_new_instance(6, ary, cJoyBallEvent);
        }
        break;
    case SDL_JOYBUTTONUP:
    case SDL_JOYBUTTONDOWN:
        {
            VALUE ary[5];
            ary[0] = UINT2NUM(event->jbutton.type);
            ary[1] = UINT2NUM(event->jbutton.timestamp);
            ary[2] = INT2NUM(event->jbutton.which);
            ary[3] = UINT2NUM(event->jbutton.button);
            ary[4] = UINT2NUM(event->jbutton.state);
            return rb_class_new_instance(5, ary, cJoyButtonEvent);
        }
        break;
    case SDL_JOYDEVICEADDED:
    case SDL_JOYDEVICEREMOVED:
        {
            VALUE ary[3];
            ary[0] = UINT2NUM(event->jdevice.type);
            ary[1] = UINT2NUM(event->jdevice.timestamp);
            ary[2] = INT2NUM(event->jdevice.which);
            return rb_class_new_instance(3, ary, cJoyDeviceEvent);
        }
        break;
    case SDL_JOYHATMOTION:
        {
            VALUE ary[5];
            ary[0] = UINT2NUM(event->jhat.type);
            ary[1] = UINT2NUM(event->jhat.timestamp);
            ary[2] = INT2NUM(event->jhat.which);
            ary[3] = UINT2NUM(event->jhat.hat);
            ary[4] = UINT2NUM(event->jhat.value);
            return rb_class_new_instance(5, ary, cJoyHatEvent);
        }
        break;
    case SDL_WINDOWEVENT:
        {
            VALUE ary[6];
            ary[0] = UINT2NUM(event->window.type);
            ary[1] = UINT2NUM(event->window.timestamp);
            ary[2] = UINT2NUM(event->window.windowID);
            ary[3] = UINT2NUM(event->window.event);
            ary[4] = INT2NUM(event->window.data1);
            ary[5] = INT2NUM(event->window.data2);
            return rb_class_new_instance(6, ary, cWindowEvent);
        }
        break;
    case SDL_DROPFILE:
        {
            VALUE ary[3];
            ary[0] = UINT2NUM(event->drop.type);
            ary[1] = UINT2NUM(event->drop.timestamp);
            ary[2] = SDL2R_TO_UTF8_STRING(event->drop.file);
            return rb_class_new_instance(3, ary, cDropEvent);
        }
        break;
    case SDL_TEXTINPUT:
        {
            VALUE ary[4];
            ary[0] = UINT2NUM(event->text.type);
            ary[1] = UINT2NUM(event->text.timestamp);
            ary[2] = UINT2NUM(event->text.windowID);
            ary[3] = SDL2R_TO_UTF8_STRING(event->text.text);
            return rb_class_new_instance(4, ary, cTextInputEvent);
        }
        break;
    case SDL_TEXTEDITING:
        {
            VALUE ary[6];
            ary[0] = UINT2NUM(event->edit.type);
            ary[1] = UINT2NUM(event->edit.timestamp);
            ary[2] = UINT2NUM(event->edit.windowID);
            ary[3] = SDL2R_TO_UTF8_STRING(event->edit.text);
            ary[4] = INT2NUM(event->edit.start);
            ary[5] = INT2NUM(event->edit.length);
            return rb_class_new_instance(6, ary, cTextEditingEvent);
        }
        break;
    case SDL_APP_TERMINATING:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cAppTerminating);
        }
        break;
    case SDL_APP_LOWMEMORY:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cAppLowMemory);
        }
        break;
    case SDL_APP_WILLENTERBACKGROUND:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cAppWillEnterBackground);
        }
        break;
    case SDL_APP_DIDENTERBACKGROUND:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cAppDidEnterBackground);
        }
        break;
    case SDL_APP_WILLENTERFOREGROUND:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cAppWillEnterForeground);
        }
        break;
    case SDL_APP_DIDENTERFOREGROUND:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cAppDidEnterForeground);
        }
        break;
    case SDL_SYSWMEVENT:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cSysWMEvent);
        }
        break;
    case SDL_CLIPBOARDUPDATE:
        {
            VALUE ary[1];
            ary[0] = UINT2NUM(event->type);
            return rb_class_new_instance(1, ary, cClipboardUpdateEvent);
        }
        break;
    case SDL_FINGERMOTION:
    case SDL_FINGERDOWN:
    case SDL_FINGERUP:
        {
            VALUE ary[9];
            ary[0] = UINT2NUM(event->tfinger.type);
            ary[1] = UINT2NUM(event->tfinger.timestamp);
            ary[2] = LL2NUM(event->tfinger.touchId);
            ary[3] = LL2NUM(event->tfinger.fingerId);
            ary[4] = DBL2NUM(event->tfinger.x);
            ary[5] = DBL2NUM(event->tfinger.y);
            ary[6] = DBL2NUM(event->tfinger.dx);
            ary[7] = DBL2NUM(event->tfinger.dy);
            ary[8] = DBL2NUM(event->tfinger.pressure);
            return rb_class_new_instance(9, ary, cTouchFingerEvent);
        }
        break;
    case SDL_MULTIGESTURE:
        {
            VALUE ary[8];
            ary[0] = UINT2NUM(event->mgesture.type);
            ary[1] = UINT2NUM(event->mgesture.timestamp);
            ary[2] = LL2NUM(event->mgesture.touchId);
            ary[3] = DBL2NUM(event->mgesture.dTheta);
            ary[4] = DBL2NUM(event->mgesture.dDist);
            ary[5] = DBL2NUM(event->mgesture.x);
            ary[6] = DBL2NUM(event->mgesture.y);
            ary[7] = INT2NUM(event->mgesture.numFingers);
            return rb_class_new_instance(8, ary, cMultiGestureEvent);
        }
        break;
    case SDL_DOLLARGESTURE:
        {
            VALUE ary[8];
            ary[0] = UINT2NUM(event->dgesture.type);
            ary[1] = UINT2NUM(event->dgesture.timestamp);
            ary[2] = LL2NUM(event->dgesture.touchId);
            ary[3] = LL2NUM(event->dgesture.gestureId);
            ary[4] = INT2NUM(event->dgesture.numFingers);
            ary[5] = DBL2NUM(event->dgesture.error);
            ary[6] = DBL2NUM(event->dgesture.x);
            ary[7] = DBL2NUM(event->dgesture.y);
            return rb_class_new_instance(8, ary, cDollarGestureEvent);
        }
        break;
    case SDL_CONTROLLERAXISMOTION:
        {
            VALUE ary[5];
            ary[0] = UINT2NUM(event->caxis.type);
            ary[1] = UINT2NUM(event->caxis.timestamp);
            ary[2] = INT2NUM(event->caxis.which);
            ary[3] = UINT2NUM(event->caxis.axis);
            ary[4] = INT2NUM(event->caxis.value);
            return rb_class_new_instance(5, ary, cControllerAxisEvent);
        }
        break;
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        {
            VALUE ary[5];
            ary[0] = UINT2NUM(event->cbutton.type);
            ary[1] = UINT2NUM(event->cbutton.timestamp);
            ary[2] = INT2NUM(event->cbutton.which);
            ary[3] = UINT2NUM(event->cbutton.button);
            ary[4] = UINT2NUM(event->cbutton.state);
            return rb_class_new_instance(5, ary, cControllerButtonEvent);
        }
        break;
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    case SDL_CONTROLLERDEVICEREMAPPED:
        {
            VALUE ary[3];
            ary[0] = UINT2NUM(event->cdevice.type);
            ary[1] = UINT2NUM(event->cdevice.timestamp);
            ary[2] = INT2NUM(event->cdevice.which);
            return rb_class_new_instance(3, ary, cControllerDeviceEvent);
        }
        break;
    default:
        if (event->type >= SDL_USEREVENT) {
            VALUE vevent = rb_hash_lookup(g_user_event_data, INT2NUM(event->user.code));
            if (vevent == Qnil) {
                VALUE ary[6];
                ary[0] = UINT2NUM(event->user.type);
                ary[1] = UINT2NUM(event->user.timestamp);
                ary[2] = UINT2NUM(event->user.windowID);
                ary[3] = INT2NUM(event->user.code);
                ary[4] = Qnil;
                ary[5] = Qnil;
                vevent = rb_class_new_instance(6, ary, cUserEvent);
            } else {
                rb_hash_delete(g_user_event_data, INT2NUM(event->user.code));
                rb_struct_aset(vevent, INT2FIX(1), INT2NUM(event->user.timestamp));
            }
            return vevent;
        }
        break;
    }

    return Qnil;
}


static VALUE sdl2r_poll_event(VALUE klass)
{
    SDL_Event event;
    VALUE vresult = Qnil;

    if (SDL_PollEvent(&event)) {
        vresult = sdl2r_make_event(&event);
    }

    return vresult;
}


static VALUE sdl2r_push_event(VALUE klass, VALUE vevent)
{
    SDL_Event ev;
    int result;

    if (NUM2UINT(rb_struct_aref(vevent, INT2FIX(0))) < SDL_USEREVENT) {
        rb_raise(eSDL2RError, "not userevent");
    }

    ev.user.type = NUM2UINT(rb_struct_aref(vevent, INT2FIX(0)));
    ev.user.code = g_user_event_id;
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


static VALUE sdl2r_wait_event(VALUE klass, VALUE vflag)
{
    SDL_Event event;
    int result;

    if (RTEST(vflag)) {
        result = SDL_WaitEvent(&event);
    } else {
        result = SDL_WaitEvent(NULL);
    }

    if (!result) {
        rb_raise(eSDLError, SDL_GetError());
    }

    if (RTEST(vflag)) {
        return sdl2r_make_event(&event);
    } else {
        return Qnil;
    }
}


static Uint32 sdl2r_timer_callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = (Uint32)param;
    userevent.code = 0;
    userevent.data1 = 0;
    userevent.data2 = 0;

    event.type = (Uint32)param;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}


static VALUE sdl2r_add_timer_event(VALUE klass, VALUE vinterval, VALUE vevent_type)
{
    SDL_TimerID timer_id;

    timer_id = SDL_AddTimer(NUM2UINT(vinterval), sdl2r_timer_callback, (VOID *)(VALUE)NUM2UINT(vevent_type));
    if (!timer_id) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(timer_id);
}


static VALUE sdl2r_remove_timer_event(VALUE klass, VALUE vtimer_id)
{
    SDL_bool result;

    result = SDL_RemoveTimer(NUM2UINT(vtimer_id));

    return SDL2R_TO_BOOL(result);
}


void Init_sdl2r_event(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, poll_event, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, push_event, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, register_events, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, event_state, 2);

//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, add_event_watch, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, del_event_watch, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, filter_events, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, flush_event, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, flush_events, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_event_filter, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_num_touch_devices, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_num_touch_fingers, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_touch_device, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, get_touch_finger, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, has_event, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, has_events, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, load_dollar_templates, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, peep_events, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, pump_events, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, quit_requested, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, record_gesture, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, save_all_dollar_templates, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, save_dollar_template, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, set_event_filter, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, wait_event, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, wait_event_timeout, 0);

    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, add_timer_event, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, remove_timer_event, 1);

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

    cDropEvent = rb_struct_define(NULL, "type", "timestamp", "file", NULL);
    rb_define_const(mSDL, "DropEvent", cDropEvent);

    cTextInputEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "text", NULL);
    rb_define_const(mSDL, "TextInputEvent", cTextInputEvent);

    cTextEditingEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "text", "start", "length", NULL);
    rb_define_const(mSDL, "TextEditingEvent", cTextEditingEvent);

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

    cControllerAxisEvent = rb_struct_define(NULL, "type", "timestamp", "which", "axis", "value", NULL);
    rb_define_const(mSDL, "ControllerAxisEvent", cControllerAxisEvent);

    cControllerButtonEvent = rb_struct_define(NULL, "type", "timestamp", "which", "button", "state", NULL);
    rb_define_const(mSDL, "ControllerButtonEvent", cControllerButtonEvent);

    cControllerDeviceEvent = rb_struct_define(NULL, "type", "timestamp", "which", NULL);
    rb_define_const(mSDL, "ControllerDeviceEvent", cControllerDeviceEvent);

    cDollarGestureEvent = rb_struct_define(NULL, "type", "timestamp", "touch_id", "gesture_id", "num_fingers", "error", "x", "y", NULL);
    rb_define_const(mSDL, "DollarGestureEvent", cDollarGestureEvent);

    cTouchFingerEvent = rb_struct_define(NULL, "type", "timestamp", "touch_id", "finger_id", "x", "y", "dx", "dy", "pressure", NULL);
    rb_define_const(mSDL, "TouchFingerEvent", cTouchFingerEvent);

    cMultiGestureEvent = rb_struct_define(NULL, "type", "timestamp", "window_id", "d_theta", "d_dist", "x", "y", "num_fingers", NULL);
    rb_define_const(mSDL, "MultiGestureEvent", cMultiGestureEvent);

    cSysWMEvent = rb_struct_define(NULL, "type", "timestamp", "msg", NULL);
    rb_define_const(mSDL, "SysWMEvent", cSysWMEvent);

    cClipboardUpdateEvent = rb_struct_define(NULL, "type", NULL);
    rb_define_const(mSDL, "ClipboardUpdateEvent", cClipboardUpdateEvent);

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


