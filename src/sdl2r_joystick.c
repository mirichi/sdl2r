#define SDL2RJOYSTICK
#include "sdl2r.h"
#include "sdl2r_hash.h"
#include "sdl2r_joystick.h"

VALUE cJoystick;
struct SDL2RHash *sdl2r_joystick_hash;

static void sdl2r_joystick_free(void *ptr);

const rb_data_type_t sdl2r_joystick_data_type = {
    "Joystick",
    {
    0,
    sdl2r_joystick_free,
    0,
    },
};


void sdl2r_dispose_joystick(struct SDL2RJoystick *js)
{
    sdl2r_del_hash(sdl2r_joystick_hash, (HASHKEY)js->joystick);
    SDL_JoystickClose(js->joystick);
    js->joystick = 0;
}


static void sdl2r_joystick_free(void *ptr)
{
    struct SDL2RJoystick *js = ptr;

    if (js->joystick) {
        sdl2r_dispose_joystick(js);
    }

    xfree(js);
}


VALUE sdl2r_joystick_alloc(VALUE klass)
{
    struct SDL2RJoystick *js;
    VALUE vjoystick = TypedData_Make_Struct(klass, struct SDL2RJoystick, &sdl2r_joystick_data_type, js);
    js->joystick = 0;
    return vjoystick;
}


static VALUE sdl2r_joystick_get_destroyed(VALUE self)
{
    struct SDL2RJoystick *js = SDL2R_GET_STRUCT(Joystick, self);
    return js->joystick ? Qfalse : Qtrue;
}


static VALUE sdl2r_num_joysticks(VALUE klass)
{
    int result = SDL_NumJoysticks();

    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_joystick_open(VALUE klass, VALUE vindex)
{
    VALUE vjoystick = sdl2r_joystick_alloc(cJoystick);
    struct SDL2RJoystick *js = SDL2R_GET_STRUCT(Joystick, vjoystick);

    SDL2R_RETRY(js->joystick = SDL_JoystickOpen(NUM2INT(vindex)));
    if (!js->joystick) {
        rb_raise(eSDLError, SDL_GetError());
    }
    sdl2r_put_hash(sdl2r_joystick_hash, (HASHKEY)js->joystick, vjoystick);

    return vjoystick;
}


static VALUE sdl2r_joystick_close(VALUE klass, VALUE vjoystick)
{
    sdl2r_dispose_joystick(SDL2R_GET_JOYSTICK_STRUCT(vjoystick));
    return vjoystick;
}


static VALUE sdl2r_joystick_num_axes(VALUE klass, VALUE vjoystick)
{
    struct SDL2RJoystick *js = SDL2R_GET_JOYSTICK_STRUCT(vjoystick);
    int result;

    result = SDL_JoystickNumAxes(js->joystick);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_joystick_num_balls(VALUE klass, VALUE vjoystick)
{
    struct SDL2RJoystick *js = SDL2R_GET_JOYSTICK_STRUCT(vjoystick);
    int result;

    result = SDL_JoystickNumBalls(js->joystick);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_joystick_num_buttons(VALUE klass, VALUE vjoystick)
{
    struct SDL2RJoystick *js = SDL2R_GET_JOYSTICK_STRUCT(vjoystick);
    int result;

    result = SDL_JoystickNumButtons(js->joystick);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_joystick_num_hats(VALUE klass, VALUE vjoystick)
{
    struct SDL2RJoystick *js = SDL2R_GET_JOYSTICK_STRUCT(vjoystick);
    int result;

    result = SDL_JoystickNumHats(js->joystick);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_joystick_event_state(VALUE klass, VALUE vstate)
{
    int result;

    result = SDL_JoystickEventState(NUM2INT(vstate));
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_joystick_name(VALUE klass, VALUE vjoystick)
{
    struct SDL2RJoystick *js = SDL2R_GET_JOYSTICK_STRUCT(vjoystick);
    const char* result;

    result = SDL_JoystickName(js->joystick);
    if (!result) {
        return Qnil;
    }

    return SDL2R_TO_UTF8_STRING(result);
}


static VALUE sdl2r_joystick_instance_id(VALUE klass, VALUE vjoystick)
{
    struct SDL2RJoystick *js = SDL2R_GET_JOYSTICK_STRUCT(vjoystick);
    SDL_JoystickID result;

    result = SDL_JoystickInstanceID(js->joystick);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(result);
}


void Init_sdl2r_joystick(void)
{
    // SDL module methods
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, num_joysticks, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_open, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_close, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_num_axes, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_num_balls, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_num_buttons, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_num_hats, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_event_state, 1);

//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_attached, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_axis, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_ball, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_button, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_device_guid, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_guid, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_guid_from_string, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_guid_string, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_get_hat, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_instance_id, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_name, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_name_for_index, 0);
//    SDL2R_DEFINE_SINGLETON_METHOD(mSDL, joystick_update, 0);

    // SDL::Joystick class
    cJoystick = rb_define_class_under(mSDL, "Joystick", rb_cObject);
    rb_define_alloc_func(cJoystick, sdl2r_joystick_alloc);
    rb_define_method(cJoystick, "destroyed?", sdl2r_joystick_get_destroyed, 0);

    sdl2r_joystick_hash = sdl2r_hash_alloc(8);
}
