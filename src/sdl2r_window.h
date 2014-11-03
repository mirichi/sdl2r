struct SDL2RWindow {
    SDL_Window *window;
    VALUE vrenderer; // weak ref
    VALUE vglcontext; // weak ref
};

void Init_sdl2r_window(void);
void sdl2r_window_dispose(struct SDL2RWindow *win);
VALUE sdl2r_window_alloc(VALUE klass);
VALUE sdl2r_window_sdl_to_ruby(SDL_Window *w);

#ifndef SDL2RWINDOW
extern struct SDL2RHash *sdl2r_window_hash;
extern const rb_data_type_t sdl2r_window_data_type;
extern VALUE cWindow;
#endif

#define SDL2R_GET_WINDOW_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_window_data_type) ?\
            (SDL2R_GET_STRUCT(Window, obj)->window == NULL ?\
                rb_raise(eSDL2RError, "disposed Window object"), SDL2R_GET_STRUCT(Window, obj)\
              : SDL2R_GET_STRUCT(Window, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Window)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Window, obj)\
    )


