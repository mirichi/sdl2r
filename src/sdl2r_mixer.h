#include <SDL_mixer.h>

struct SDL2RChunk {
    Mix_Chunk *chunk;
};
struct SDL2RMusic {
    Mix_Music *music;
};

void Init_sdl2r_mixer(void);
VALUE sdl2r_chunk_alloc(VALUE klass);
VALUE sdl2r_music_alloc(VALUE klass);

#ifndef SDL2RMIXER
extern VALUE mMixer;
extern const rb_data_type_t sdl2r_chunk_data_type;
extern VALUE cChunk;
extern const rb_data_type_t sdl2r_music_data_type;
extern VALUE cMusic;
#endif

#define SDL2R_GET_CHUNK_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_chunk_data_type) ?\
            ((SDL2R_GET_STRUCT(Chunk, obj)->chunk == NULL) ?\
                rb_raise(eSDL2RError, "destroyed Chunk object"), SDL2R_GET_STRUCT(Chunk, obj)\
              : SDL2R_GET_STRUCT(Chunk, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Mix::Chunk)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Chunk, obj)\
    )
#define SDL2R_GET_MUSIC_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_music_data_type) ?\
            ((SDL2R_GET_STRUCT(Music, obj)->music == NULL) ?\
                rb_raise(eSDL2RError, "destroyed Music object"), SDL2R_GET_STRUCT(Music, obj)\
              : SDL2R_GET_STRUCT(Music, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Mix::Music)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Music, obj)\
    )


