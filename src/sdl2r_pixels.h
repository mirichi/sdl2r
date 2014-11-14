struct SDL2RPixels {
    VALUE vsurface;
};

void Init_sdl2r_pixels(void);
VALUE sdl2r_pixels_alloc(VALUE klass);

#ifndef SDL2RPIXELS
extern const rb_data_type_t sdl2r_pixels_data_type;
extern VALUE cPixels;
#endif

#define SDL2R_GET_PIXELS_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_pixels_data_type) ?\
            SDL2R_GET_STRUCT(Pixels, obj)\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Surface::Pixels)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Pixels, obj)\
    )




