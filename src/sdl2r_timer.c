#define SDL2RTIMER
#include "sdl2r.h"
#include "sdl2r_timer.h"


VALUE sdl2r_delay(VALUE klass, VALUE vms)
{
    Sleep(NUM2INT(vms));
    return Qnil;
}


VALUE sdl2r_get_ticks(VALUE klass)
{
    Uint32 ticks = SDL_GetTicks();
    return UINT2NUM(ticks);
}


VALUE sdl2r_get_performance_counter(VALUE klass)
{
    Uint64 count = SDL_GetPerformanceCounter();
    return LL2NUM(count);
}


VALUE sdl2r_get_performance_frequency(VALUE klass)
{
    Uint64 freq = SDL_GetPerformanceFrequency();
    return LL2NUM(freq);
}


void Init_sdl2r_timer(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "delay", sdl2r_delay, 1);
    rb_define_singleton_method(mSDL, "get_ticks", sdl2r_get_ticks, 0);
    rb_define_singleton_method(mSDL, "get_performance_counter", sdl2r_get_performance_counter, 0);
    rb_define_singleton_method(mSDL, "get_performance_frequency", sdl2r_get_performance_frequency, 0);
}


