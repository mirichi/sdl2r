#define SDL2RTIMER
#include "sdl2r.h"
#include "sdl2r_timer.h"


VALUE sdl2r_delay(VALUE klass, VALUE vms)
{
//    Sleep(NUM2INT(vms));
    rb_thread_wait_for(rb_time_interval(rb_float_new(NUM2DBL(vms)/1000.0)));
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
    SDL2R_DEFINE_SINGLETON_METHOD(delay, 1);
    SDL2R_DEFINE_SINGLETON_METHOD(get_ticks, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(get_performance_counter, 0);
    SDL2R_DEFINE_SINGLETON_METHOD(get_performance_frequency, 0);
}
