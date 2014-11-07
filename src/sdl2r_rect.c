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
    SDL_bool bol = SDL_FALSE;
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
        bol = SDL_EnclosePoints(points, RARRAY_LEN(vpoints), pclip, &result);
    }

    if (bol == SDL_FALSE) {
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
    SDL2R_SET_RECT(b, vrectb);

    return SDL2R_TO_BOOL(SDL_HasIntersection(&a, &b));
}


VALUE sdl2r_intersect_rect(VALUE klass, VALUE vrecta, VALUE vrectb)
{
    SDL_Rect a, b, result;
    VALUE ary[4];
    SDL_bool bol;

    SDL2R_SET_RECT(a, vrecta);
    SDL2R_SET_RECT(b, vrectb);

    bol = SDL_IntersectRect(&a, &b, &result);
    if (bol == SDL_FALSE) {
        return Qnil;
    }

    ary[0] = INT2NUM(result.x);
    ary[1] = INT2NUM(result.y);
    ary[2] = INT2NUM(result.w);
    ary[3] = INT2NUM(result.h);
    return rb_class_new_instance(4, ary, cRect);
}


VALUE sdl2r_intersect_rect_and_line(VALUE klass, VALUE vrect, VALUE vpointa, VALUE vpointb)
{
    SDL_Rect rect;
    SDL_Point a, b;
    VALUE ary_a[2];
    VALUE ary_b[2];
    SDL_bool bol;

    SDL2R_SET_RECT(rect, vrect);
    SDL2R_SET_POINT(a, vpointa);
    SDL2R_SET_POINT(b, vpointb);

    bol = SDL_IntersectRectAndLine(&rect, &a.x, &a.y, &b.x, &b.y);
    if (bol == SDL_FALSE) {
        return Qnil;
    }

    ary_a[0] = INT2NUM(a.x);
    ary_a[1] = INT2NUM(a.y);
    ary_b[0] = INT2NUM(b.x);
    ary_b[1] = INT2NUM(b.y);
    return rb_ary_new3(2, rb_class_new_instance(2, ary_a, cPoint), rb_class_new_instance(2, ary_b, cPoint));
}


//VALUE sdl2r_point_in_rect(VALUE klass, VALUE vpoint, VALUE vrect)
//{
//    SDL_Point point;
//    SDL_Rect rect;
//    SDL_bool bol;
//
//    SDL2R_SET_POINT(point, vpoint);
//    SDL2R_SET_RECT(rect, vrect);
//
//    bol = SDL_PointInRect(&point, &rect);
//    return SDL2R_TO_BOOL(bol);
//}


VALUE sdl2r_rect_empty(VALUE klass, VALUE vrect)
{
    SDL_Rect rect;

    SDL2R_SET_RECT(rect, vrect);

    return SDL2R_TO_BOOL(SDL_RectEmpty(&rect));
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
    SDL2R_DEFINE_SINGLETON_METHOD(intersect_rect, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(intersect_rect_and_line, 3);
//    SDL2R_DEFINE_SINGLETON_METHOD(point_in_rect, 2);
    SDL2R_DEFINE_SINGLETON_METHOD(rect_empty, 1);
//    SDL2R_DEFINE_SINGLETON_METHOD(rect_equals, 2);
//    SDL2R_DEFINE_SINGLETON_METHOD(union_rect, 2);
}


