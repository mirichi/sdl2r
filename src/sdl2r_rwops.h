struct SDL2RRWops {
    SDL_RWops *rwops;
    VALUE vstr;
};

void Init_sdl2r_rwops(void);
VALUE sdl2r_rwops_alloc(VALUE klass);

#ifndef SDL2RRWOPS
extern const rb_data_type_t sdl2r_rwops_data_type;
extern VALUE cRWops;
#endif

#define SDL2R_GET_RWOPS_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_rwops_data_type) ?\
            ((SDL2R_GET_STRUCT(RWops, obj)->rwops == NULL) ?\
                rb_raise(eSDL2RError, "destroyed RWops object"), SDL2R_GET_STRUCT(RWops, obj)\
                : SDL2R_GET_STRUCT(RWops, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::RWops)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(RWops, obj)\
    )
