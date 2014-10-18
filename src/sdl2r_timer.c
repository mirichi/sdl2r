#include <SDL.h>

#define SDL2RTIMER
#include "sdl2r.h"
#include "sdl2r_timer.h"


VALUE sdl2r_delay(VALUE klass, VALUE vms)
{
    SDL_Delay(NUM2INT(vms));
    return Qnil;
}

VALUE sdl2r_get_ticks(VALUE klass)
{
    Uint32 ticks = SDL_GetTicks();
    return UINT2NUM(ticks);
}

void Init_sdl2r_timer(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "delay", sdl2r_delay, 1);
    rb_define_singleton_method(mSDL, "get_ticks", sdl2r_get_ticks, 0);
}


