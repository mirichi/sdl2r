#define SDL2RRECT
#include "sdl2r.h"
#include "sdl2r_rect.h"

VALUE cRect;
VALUE cPoint;

VALUE sdl2r_has_intersection(VALUE klass, VALUE vrecta, VALUE vrectb)
{
    SDL_Rect a, b;

    Check_Type(vrecta, T_ARRAY);
    a.x = NUM2INT(rb_ary_entry(vrecta, 0));
    a.y = NUM2INT(rb_ary_entry(vrecta, 1));
    a.w = NUM2INT(rb_ary_entry(vrecta, 2));
    a.h = NUM2INT(rb_ary_entry(vrecta, 3));

    Check_Type(vrectb, T_ARRAY);
    b.x = NUM2INT(rb_ary_entry(vrectb, 0));
    b.y = NUM2INT(rb_ary_entry(vrectb, 1));
    b.w = NUM2INT(rb_ary_entry(vrectb, 2));
    b.h = NUM2INT(rb_ary_entry(vrectb, 3));

    return SDL_HasIntersection(&a, &b) ? Qtrue : Qfalse;
}


void Init_sdl2r_rect(void)
{
    // SDL::Point class
    cPoint = rb_define_class_under(mSDL, "Point", rb_cArray);

    // SDL::Rect class
    cRect = rb_define_class_under(mSDL, "Rect", rb_cArray);

    // SDL module methods
//    rb_define_singleton_method(mSDL, "enclose_points", sdl2r_enclose_points, 2);
    rb_define_singleton_method(mSDL, "has_intersection", sdl2r_has_intersection, 2);
//    rb_define_singleton_method(mSDL, "intersect_rect", sdl2r_intersect_rect, 2);
//    rb_define_singleton_method(mSDL, "intersect_rect_and_line", sdl2r_intersect_rect_and_line, 3);
//    rb_define_singleton_method(mSDL, "point_in_rect", sdl2r_point_in_rect, 2);
//    rb_define_singleton_method(mSDL, "rect_rmpty", sdl2r_rect_rmpty, 1);
//    rb_define_singleton_method(mSDL, "rect_rquals", sdl2r_rect_rquals, 2);
//    rb_define_singleton_method(mSDL, "union_rect", sdl2r_union_rect, 2);
}


