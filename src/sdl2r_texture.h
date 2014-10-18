struct SDL2RTexture {
    SDL_Texture *texture;
    VALUE vrenderer;
};

void Init_sdl2r_texture(void);
VALUE sdl2r_texture_alloc(VALUE klass);

#ifndef SDL2RTEXTURE
extern const rb_data_type_t sdl2r_texture_data_type;
extern VALUE cTexture;
#endif

#define SDL2R_GET_TEXTURE_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_texture_data_type) ?\
            ((SDL2R_GET_STRUCT(Texture, obj)->texture == NULL) ?\
                rb_raise(eSDL2RError, "destroyed Texture object"), SDL2R_GET_STRUCT(Texture, obj)\
              : SDL2R_GET_STRUCT(Texture, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Texture)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Texture, obj)\
    )


