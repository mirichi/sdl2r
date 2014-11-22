void Init_sdl2r_rect(void);

#ifndef SDL2R
extern VALUE cPoint;
extern VALUE cRect;
#endif

VALUE sdl2r_rect2vrect(SDL_Rect *r);
VALUE sdl2r_point2vpoint(SDL_Point *p);

#define SDL2R_SET_POINT(point, vpoint) {\
            Check_Type(vpoint, T_ARRAY);\
            point.x = NUM2INT(rb_ary_entry(vpoint, 0));\
            point.y = NUM2INT(rb_ary_entry(vpoint, 1));\
        }
#define SDL2R_SET_RECT(rect, vrect) {\
            Check_Type(vrect, T_ARRAY);\
            rect.x = NUM2INT(rb_ary_entry(vrect, 0));\
            rect.y = NUM2INT(rb_ary_entry(vrect, 1));\
            rect.w = NUM2INT(rb_ary_entry(vrect, 2));\
            rect.h = NUM2INT(rb_ary_entry(vrect, 3));\
        }
#define SDL2R_SET_RECT_OR_NULL(prect, rect, vrect) {\
            if (vrect == Qnil) {\
                prect = NULL;\
            } else {\
                Check_Type(vrect, T_ARRAY);\
                rect.x = NUM2INT(rb_ary_entry(vrect, 0));\
                rect.y = NUM2INT(rb_ary_entry(vrect, 1));\
                rect.w = NUM2INT(rb_ary_entry(vrect, 2));\
                rect.h = NUM2INT(rb_ary_entry(vrect, 3));\
                prect = &rect;\
            }\
        }

