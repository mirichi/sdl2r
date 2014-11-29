struct SDL2RPixelFormat {
    SDL_PixelFormat *format;
};

void Init_sdl2r_pixelformat(void);
VALUE sdl2r_pixelformat_alloc(VALUE klass);

#ifndef SDL2RPIXELFORMAT
extern const rb_data_type_t sdl2r_pixelformat_data_type;
extern VALUE cPixelFormat;
#endif

#define SDL2R_GET_PIXELFORMAT_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_pixelformat_data_type) ?\
            (SDL2R_GET_STRUCT(PixelFormat, obj)->format == NULL ?\
                rb_raise(eSDL2RError, "disposed PixelFormat object"), SDL2R_GET_STRUCT(PixelFormat, obj)\
              : SDL2R_GET_STRUCT(PixelFormat, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Surface::PixelFormat)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(PixelFormat, obj)\
    )




