#define SDL2RRECT
#include "sdl2r.h"
#include "sdl2r_rect.h"

VALUE cRect;
VALUE cPoint;

VALUE sdl2r_has_intersection(VALUE klass, VALUE vrecta, VALUE vrectb)
{
    SDL_Rect a, b;

    SDL2R_SET_RECT(a, vrecta);
    Check_Type(vrecta, T_ARRAY);

    SDL2R_SET_RECT(b, vrectb);

    return SDL_HasIntersection(&a, &b) ? Qtrue : Qfalse;
}


void Init_sdl2r_rect(void)
{
    // SDL::Point class
    cPoint = rb_define_class_under(mSDL, "Point", rb_cArray);

    // SDL::Rect class
    cRect = rb_define_class_under(mSDL, "Rect", rb_cArray);

    // SDL module methods
//    SDL2R_DEFINE_SINGLETON_METHOD(enclose_points, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(has_intersection, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(intersect_rect, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(intersect_rect_and_line, 3);
//    SDL2R_DEFINE_SINGLETON_METHOD(point_in_rect, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(rect_rmpty, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(rect_rquals, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(union_rect, 2);
}


