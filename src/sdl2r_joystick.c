#include <SDL.h>

#define SDL2RJOYSTICK
#include "sdl2r.h"
#include "sdl2r_joystick.h"

VALUE cJoystick;

static void sdl2r_joystick_free(void *ptr);

const rb_data_type_t sdl2r_joystick_data_type = {
    "Joystick",
    {
    0,
    sdl2r_joystick_free,
    0,
    },
};


static void sdl2r_joystick_free(void *ptr)
{
    struct SDL2RJoystick *js = ptr;

    if (js->joystick) {
        SDL_JoystickClose(js->joystick);
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


void Init_sdl2r_joystick(void)
{
    cJoystick = rb_define_module_under(mSDL, "Joystick");
}
