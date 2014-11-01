struct SDL2RJoystick {
    SDL_Joystick *joystick;
};

void Init_sdl2r_joystick(void);
void sdl2r_joystick_dispose(struct SDL2RJoystick *js);
VALUE sdl2r_joystick_alloc(VALUE klass);

#ifndef SDL2RJOYSTICK
extern struct SDL2RHash *sdl2r_joystick_hash;
extern const rb_data_type_t sdl2r_joystick_data_type;
extern VALUE cJoystick;
#endif

#define SDL2R_GET_JOYSTICK_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_joystick_data_type) ?\
            ((SDL2R_GET_STRUCT(Joystick, obj)->joystick == NULL) ?\
                rb_raise(eSDL2RError, "destroyed Joystick object"), SDL2R_GET_STRUCT(Joystick, obj)\
              : SDL2R_GET_STRUCT(Joystick, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Joystick)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Joystick, obj)\
    )


