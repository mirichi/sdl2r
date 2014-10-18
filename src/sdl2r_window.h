struct SDL2RWindow {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void Init_sdl2r_window(void);
VALUE sdl2r_window_alloc(VALUE klass);

#ifndef SDL2RWINDOW
extern const rb_data_type_t sdl2r_window_data_type;
extern VALUE cWindow;
#endif

#define SDL2R_GET_WINDOW_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_window_data_type) ?\
            ((SDL2R_GET_STRUCT(Window, obj)->window == NULL) ?\
                rb_raise(eSDL2RError, "destroyed Window object"), SDL2R_GET_STRUCT(Window, obj)\
              : SDL2R_GET_STRUCT(Window, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Window)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Window, obj)\
    )


