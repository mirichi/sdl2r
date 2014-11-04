#define SDL2RRECT
#include "sdl2r.h"
#include "sdl2r_rect.h"

VALUE cRect;
VALUE cPoint;

VALUE sdl2r_enclose_points(VALUE klass, VALUE vpoints, VALUE vclip)
{
    SDL_Rect clip, result;
    SDL_Rect *pclip=0;
    int i;
    SDL_bool bool;
    VALUE ary[4];

    if (vclip != Qnil) {
        SDL2R_SET_RECT(clip, vclip);
        pclip = &clip;
    }

    Check_Type(vpoints, T_ARRAY);
    {
        SDL_Point points[RARRAY_LEN(vpoints)];
        for (i = 0; i < RARRAY_LEN(vpoints); i++) {
            Check_Type(rb_ary_entry(vpoints, i), T_ARRAY);
            SDL2R_SET_POINT(points[i], rb_ary_entry(vpoints, i));
        }
        bool = SDL_EnclosePoints(points, RARRAY_LEN(vpoints), pclip, &result);
    }

    if (bool == SDL_FALSE) {
        return Qnil;
    }

    ary[0] = INT2NUM(result.x);
    ary[1] = INT2NUM(result.y);
    ary[2] = INT2NUM(result.w);
    ary[3] = INT2NUM(result.h);
    return rb_class_new_instance(4, ary, cRect);
}


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
    SDL2R_DEFINE_SINGLETON_METHOD(enclose_points, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(has_intersection, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(intersect_rect, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(intersect_rect_and_line, 3);
//    SDL2R_DEFINE_SINGLETON_METHOD(point_in_rect, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(rect_rmpty, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(rect_rquals, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(union_rect, 2);
}


