struct SDL2RGLContext {
    SDL_GLContext glcontext;
    VALUE vwindow;
};

void Init_sdl2r_opengl(void);
void sdl2r_dispose_glcontext(struct SDL2RGLContext *glc);
VALUE sdl2r_glcontext_alloc(VALUE klass);


#ifndef SDL2ROPENGL
extern const rb_data_type_t sdl2r_glcontext_data_type;
extern VALUE cGLContext;
#endif

#define SDL2R_GET_GLCONTEXT_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_glcontext_data_type) ?\
            ((SDL2R_GET_STRUCT(GLContext, obj)->glcontext == NULL) ?\
                rb_raise(eSDL2RError, "destroyed GLContext object"), SDL2R_GET_STRUCT(GLContext, obj)\
              : SDL2R_GET_STRUCT(GLContext, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::GLContext)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(GLContext, obj)\
    )


