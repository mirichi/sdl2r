struct SDL2RRenderer {
    VALUE vwindow;
};

void Init_sdl2r_renderer(void);
VALUE sdl2r_renderer_alloc(VALUE klass);

#ifndef SDL2RRENDERER
extern const rb_data_type_t sdl2r_renderer_data_type;
extern VALUE cRenderer;
#endif

#define SDL2R_GET_RENDERER_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_renderer_data_type) ? \
            ((SDL2R_GET_STRUCT(Renderer, obj)->vwindow == Qnil || (SDL2R_GET_STRUCT(Window, SDL2R_GET_STRUCT(Renderer, obj)->vwindow))->renderer == NULL) ?  \
                rb_raise(eSDL2RError, "destroyed Renderer object"), SDL2R_GET_STRUCT(Renderer, obj)\
              : SDL2R_GET_STRUCT(Renderer, obj)\
            )\
      : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Renderer)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Renderer, obj)\
    )



