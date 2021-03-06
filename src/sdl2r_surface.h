struct SDL2RSurface {
    SDL_Surface *surface;
    VALUE vwindow;
    VALUE vpixels;
    VALUE vpixelformat;
};

void Init_sdl2r_surface(void);
VALUE sdl2r_surface_alloc(VALUE klass);
SDL_Surface *sdl2r_get_sdl_surface(struct SDL2RSurface *sur);

#ifndef SDL2RSURFACE
extern const rb_data_type_t sdl2r_surface_data_type;
extern VALUE cSurface;
#endif

#define SDL2R_GET_SURFACE_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_surface_data_type) ?\
            ((SDL2R_GET_STRUCT(Surface, obj)->surface == NULL && SDL2R_GET_STRUCT(Surface, obj)->vwindow == Qnil) ?\
                rb_raise(eSDL2RError, "disposed Surface object"), SDL2R_GET_STRUCT(Surface, obj)\
              : SDL2R_GET_STRUCT(Surface, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Surface)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Surface, obj)\
    )
