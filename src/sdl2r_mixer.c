#include <SDL.h>

#define SDL2RMIXER
#include "sdl2r.h"
#include "sdl2r_mixer.h"

VALUE mMixer;
VALUE cChunk;
VALUE cMusic;

static void sdl2r_chunk_free(void *ptr);
const rb_data_type_t sdl2r_chunk_data_type = {
    "Chunk",
    {
    0,
    sdl2r_chunk_free,
    0,
    },
};

static void sdl2r_music_free(void *ptr);
const rb_data_type_t sdl2r_music_data_type = {
    "Music",
    {
    0,
    sdl2r_music_free,
    0,
    },
};


static void sdl2r_chunk_free(void *ptr)
{
    struct SDL2RChunk *cnk = ptr;
    if (cnk->chunk) {
        Mix_FreeChunk(cnk->chunk);
        cnk->chunk = 0;
    }
    xfree(cnk);
}


VALUE sdl2r_chunk_alloc(VALUE klass)
{
    struct SDL2RChunk *cnk;
    VALUE vchunk = TypedData_Make_Struct(klass, struct SDL2RChunk, &sdl2r_chunk_data_type, cnk);
    cnk->chunk = 0;
    return vchunk;
}


static VALUE sdl2r_chunk_get_destroyed(VALUE self)
{
    struct SDL2RChunk *cnk = SDL2R_GET_STRUCT(Chunk, self);
    return cnk->chunk ? Qfalse : Qtrue;
}


static void sdl2r_music_free(void *ptr)
{
    struct SDL2RMusic *mus = ptr;
    if (mus->music) {
        Mix_FreeMusic(mus->music);
        mus->music = 0;
    }
    xfree(mus);
}


VALUE sdl2r_music_alloc(VALUE klass)
{
    struct SDL2RMusic *mus;
    VALUE vmusic = TypedData_Make_Struct(klass, struct SDL2RMusic, &sdl2r_music_data_type, mus);
    mus->music = 0;
    return vmusic;
}


static VALUE sdl2r_music_get_destroyed(VALUE self)
{
    struct SDL2RMusic *mus = SDL2R_GET_STRUCT(Music, self);
    return mus->music ? Qfalse : Qtrue;
}


static VALUE sdl2r_mix_open_audio(VALUE klass, VALUE vfrequency, VALUE vformat, VALUE vchannels, VALUE vchunksize)
{

    if (Mix_OpenAudio(NUM2INT(vfrequency), NUM2INT(vformat), NUM2INT(vchannels), NUM2INT(vchunksize))) {
        rb_raise(eSDLError, SDL_GetError());
    }
    return Qnil;
}


static VALUE sdl2r_mix_close_audio(VALUE klass)
{
    Mix_CloseAudio();
    return Qnil;
}


static VALUE sdl2r_mix_load_wav(VALUE klass, VALUE filename)
{
    VALUE vchunk = sdl2r_chunk_alloc(cChunk);
    struct SDL2RChunk *cnk = SDL2R_GET_STRUCT(Chunk, vchunk);

    SDL2R_RETRY(cnk->chunk = Mix_LoadWAV(RSTRING_PTR(filename)));
    if (!cnk->chunk) {
        rb_raise(eSDLError, Mix_GetError());
    }

    return vchunk;
}


static VALUE sdl2r_mix_load_mus(VALUE klass, VALUE filename)
{
    VALUE vmusic = sdl2r_music_alloc(cMusic);
    struct SDL2RMusic *mus = SDL2R_GET_STRUCT(Music, vmusic);

    SDL2R_RETRY(mus->music = Mix_LoadMUS(RSTRING_PTR(filename)));
    if (!mus->music) {
        rb_raise(eSDLError, Mix_GetError());
    }

    return vmusic;
}


static VALUE sdl2r_mix_play_channel(VALUE klass, VALUE vchannel, VALUE vchunk, VALUE vloops)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    int result;

    result = Mix_PlayChannel(NUM2INT(vchannel), cnk->chunk, NUM2INT(vloops));
    if (result == -1) {
        rb_raise(eSDLError, Mix_GetError());
    }

    return INT2NUM(result);
}


static VALUE sdl2r_mix_play_music(VALUE klass, VALUE vmusic, VALUE vloops)
{
    struct SDL2RMusic *mus = SDL2R_GET_MUSIC_STRUCT(vmusic);

    if (Mix_PlayMusic(mus->music, NUM2INT(vloops))) {
        rb_raise(eSDLError, Mix_GetError());
    }

    return vmusic;
}


void Init_sdl2r_mixer(void)
{
    mMixer = rb_define_module_under(mSDL, "Mix");

    // Mix module methods
    rb_define_singleton_method(mMixer, "open_audio", sdl2r_mix_open_audio, 4);
    rb_define_singleton_method(mMixer, "close_audio", sdl2r_mix_close_audio, 0);
    rb_define_singleton_method(mMixer, "load_wav", sdl2r_mix_load_wav, 1);
    rb_define_singleton_method(mMixer, "load_mus", sdl2r_mix_load_mus, 1);
    rb_define_singleton_method(mMixer, "play_channel", sdl2r_mix_play_channel, 3);
    rb_define_singleton_method(mMixer, "play_music", sdl2r_mix_play_music, 2);

    // SDL::Mix::Chunk class
    cChunk = rb_define_class_under(mMixer, "Chunk", rb_cObject);
    rb_define_alloc_func(cChunk, sdl2r_chunk_alloc);

    rb_define_method(cChunk, "destroyed?", sdl2r_chunk_get_destroyed, 0);

    // SDL::Mix::Music class
    cMusic = rb_define_class_under(mMixer, "Music", rb_cObject);
    rb_define_alloc_func(cMusic, sdl2r_music_alloc);

    rb_define_method(cMusic, "destroyed?", sdl2r_music_get_destroyed, 0);

    // Constants
    rb_define_const(mMixer, "DEFAULT_FREQUENCY", INT2FIX(MIX_DEFAULT_FREQUENCY));
    rb_define_const(mMixer, "CHANNELS", INT2FIX(MIX_CHANNELS));
    rb_define_const(mMixer, "DEFAULT_FORMAT", INT2FIX(MIX_DEFAULT_FORMAT));
    rb_define_const(mSDL, "AUDIO_U8", INT2FIX(AUDIO_U8));
    rb_define_const(mSDL, "AUDIO_S8", INT2FIX(AUDIO_S8));
    rb_define_const(mSDL, "AUDIO_U16LSB", INT2FIX(AUDIO_U16LSB));
    rb_define_const(mSDL, "AUDIO_S16LSB", INT2FIX(AUDIO_S16LSB));
    rb_define_const(mSDL, "AUDIO_U16MSB", INT2FIX(AUDIO_U16MSB));
    rb_define_const(mSDL, "AUDIO_S16MSB", INT2FIX(AUDIO_S16MSB));
    rb_define_const(mSDL, "AUDIO_U16", INT2FIX(AUDIO_U16));
    rb_define_const(mSDL, "AUDIO_S16", INT2FIX(AUDIO_S16));
    rb_define_const(mSDL, "AUDIO_U16SYS", INT2FIX(AUDIO_U16SYS));
    rb_define_const(mSDL, "AUDIO_S16SYS", INT2FIX(AUDIO_S16SYS));
}


