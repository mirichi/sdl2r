#define SDL2RMIXER
#include "sdl2r.h"
#include "sdl2r_hash.h"
#include "sdl2r_mixer.h"
#include "sdl2r_rwops.h"
#include "sdl2r_version.h"

VALUE mMixer;
VALUE cChunk;
VALUE cMusic;
struct SDL2RHash *sdl2r_chunk_hash;
struct SDL2RHash *sdl2r_music_hash;

static VALUE sdl2r_EnumAudioFormat;
static VALUE sdl2r_EnumMusicType;
static VALUE sdl2r_EnumFading;

static Uint32 ChannelFinishedEventID;

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


void sdl2r_chunk_dispose(struct SDL2RChunk *cnk)
{
    sdl2r_del_hash(sdl2r_chunk_hash, (HASHKEY)cnk->chunk);
    Mix_FreeChunk(cnk->chunk);
    cnk->chunk = 0;
}


static void sdl2r_chunk_free(void *ptr)
{
    struct SDL2RChunk *cnk = ptr;
    if (cnk->chunk) {
        sdl2r_chunk_dispose(cnk);
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


static VALUE sdl2r_chunk_im_dispose(VALUE self)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(self);
    Mix_FreeChunk(cnk->chunk);
    return Qnil;
}


static VALUE sdl2r_chunk_im_get_disposed(VALUE self)
{
    struct SDL2RChunk *cnk = SDL2R_GET_STRUCT(Chunk, self);
    return cnk->chunk ? Qfalse : Qtrue;
}


static VALUE sdl2r_mix_free_chunk(VALUE klass, VALUE vchunk)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    Mix_FreeChunk(cnk->chunk);
    return Qnil;
}


void sdl2r_music_dispose(struct SDL2RMusic *mus)
{
    sdl2r_del_hash(sdl2r_music_hash, (HASHKEY)mus->music);
    Mix_FreeMusic(mus->music);
    mus->music = 0;
}


static void sdl2r_music_free(void *ptr)
{
    struct SDL2RMusic *mus = ptr;
    if (mus->music) {
        sdl2r_music_dispose(mus);
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


static VALUE sdl2r_music_im_dispose(VALUE self)
{
    struct SDL2RMusic *mus = SDL2R_GET_MUSIC_STRUCT(self);
    Mix_FreeMusic(mus->music);
    return Qnil;
}


static VALUE sdl2r_music_im_get_disposed(VALUE self)
{
    struct SDL2RMusic *mus = SDL2R_GET_STRUCT(Music, self);
    return mus->music ? Qfalse : Qtrue;
}


static VALUE sdl2r_mix_free_music(VALUE klass, VALUE vmusic)
{
    struct SDL2RMusic *mus = SDL2R_GET_MUSIC_STRUCT(vmusic);
    Mix_FreeMusic(mus->music);
    return Qnil;
}


// SDL module method
static VALUE sdl2r_mix_open_audio(VALUE klass, VALUE vfrequency, VALUE vformat, VALUE vchannels, VALUE vchunksize)
{

    if (Mix_OpenAudio(NUM2INT(vfrequency), NUM2INT(vformat), NUM2INT(vchannels), NUM2INT(vchunksize))) {
        rb_raise(eSDLError, SDL_GetError());
    }
    return Qnil;
}


static VALUE sdl2r_mix_close_audio(VALUE klass)
{
    SDL2R_CLEAR_HASH(sdl2r_chunk_hash, Chunk, chunk, sdl2r_chunk_dispose);
    SDL2R_CLEAR_HASH(sdl2r_music_hash, Music, music, sdl2r_music_dispose);
    Mix_CloseAudio();
    return Qnil;
}


static VALUE sdl2r_mix_query_spec(VALUE klass)
{
    int frequency, channels;
    Uint16 format;
    int result;

    result = Mix_QuerySpec(&frequency, &format, &channels);
    if (result == 0) {
        return Qnil;
    }

    return rb_ary_new3(3, INT2NUM(frequency), INT2NUM(format), INT2NUM(channels));
}


VALUE sdl2r_mix_linked_version(VALUE klass)
{
    const SDL_version *v = Mix_Linked_Version();
    VALUE ary[3];

    ary[0] = INT2NUM(v->major);
    ary[1] = INT2NUM(v->minor);
    ary[2] = INT2NUM(v->patch);

    return rb_class_new_instance(3, ary, cVersion);
}


VALUE sdl2r_macro_MIXER_VERSION(VALUE klass)
{
    SDL_version v;
    VALUE ary[3];

    SDL_MIXER_VERSION(&v);
    ary[0] = INT2NUM(v.major);
    ary[1] = INT2NUM(v.minor);
    ary[2] = INT2NUM(v.patch);

    return rb_class_new_instance(3, ary, cVersion);
}


// Chunk
static VALUE sdl2r_mix_load_wav(VALUE klass, VALUE vfilename)
{
    VALUE vchunk = sdl2r_chunk_alloc(cChunk);
    struct SDL2RChunk *cnk = SDL2R_GET_STRUCT(Chunk, vchunk);

    Check_Type(vfilename, T_STRING);
    SDL2R_RETRY(cnk->chunk = Mix_LoadWAV(RSTRING_PTR(vfilename)));
    if (!cnk->chunk) {
        rb_raise(eSDLError, Mix_GetError());
    }
    sdl2r_put_hash(sdl2r_chunk_hash, (HASHKEY)cnk->chunk, vchunk);

    return vchunk;
}


static VALUE sdl2r_mix_load_wav_rw(VALUE klass, VALUE vrwops, VALUE vfreesrc)
{
    VALUE vchunk = sdl2r_chunk_alloc(cChunk);
    struct SDL2RChunk *cnk = SDL2R_GET_STRUCT(Chunk, vchunk);
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    SDL2R_RETRY(cnk->chunk = Mix_LoadWAV_RW(rw->rwops, NUM2INT(vfreesrc)));
    if (!cnk->chunk) {
        rb_raise(eSDLError, Mix_GetError());
    }
    sdl2r_put_hash(sdl2r_chunk_hash, (HASHKEY)cnk->chunk, vchunk);

    if (NUM2INT(vfreesrc) != 0) {
        rw->rwops = 0;
        rw->vstr = Qnil;
    }

    return vchunk;
}


static void sdl2r_mix_ChannelFinishedCallBack(int channel)
{
    SDL_Event ev;
    int result;

    ev.user.type = ChannelFinishedEventID;
    ev.user.code = channel;
    result = SDL_PushEvent(&ev);
    if (result < 0) {
        rb_raise(eSDLError, SDL_GetError());
    }
}


static VALUE sdl2r_mix_play_channel(VALUE klass, VALUE vchannel, VALUE vchunk, VALUE vloops)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    int result;

    result = Mix_PlayChannel(NUM2INT(vchannel), cnk->chunk, NUM2INT(vloops));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_play_channel_timed(VALUE klass, VALUE vchannel, VALUE vchunk, VALUE vloops, VALUE ticks)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    int result;

    result = Mix_PlayChannelTimed(NUM2INT(vchannel), cnk->chunk, NUM2INT(vloops), NUM2INT(ticks));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_fade_in_channel(VALUE klass, VALUE vchannel, VALUE vchunk, VALUE vloops, VALUE ms)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    int result;

    result = Mix_FadeInChannel(NUM2INT(vchannel), cnk->chunk, NUM2INT(vloops), NUM2INT(ms));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_fade_in_channel_timed(VALUE klass, VALUE vchannel, VALUE vchunk, VALUE vloops, VALUE ms, VALUE ticks)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    int result;

    result = Mix_FadeInChannelTimed(NUM2INT(vchannel), cnk->chunk, NUM2INT(vloops), NUM2INT(ms), NUM2INT(ticks));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_volume_chunk(VALUE klass, VALUE vchunk, VALUE vvolume)
{
    struct SDL2RChunk *cnk = SDL2R_GET_CHUNK_STRUCT(vchunk);
    int result;

    result = Mix_VolumeChunk(cnk->chunk, NUM2INT(vvolume));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_allocate_channels(VALUE klass, VALUE vnumchans)
{
    int result;

    result = Mix_AllocateChannels(NUM2INT(vnumchans));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_volume(VALUE klass, VALUE vchannel, VALUE vvolume)
{
    int result;

    result = Mix_Volume(NUM2INT(vchannel), NUM2INT(vvolume));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_pause(VALUE klass, VALUE vchannel)
{
    Mix_Pause(NUM2INT(vchannel));
    return Qnil;
}


static VALUE sdl2r_mix_resume(VALUE klass, VALUE vchannel)
{
    Mix_Resume(NUM2INT(vchannel));
    return Qnil;
}


static VALUE sdl2r_mix_halt_channel(VALUE klass, VALUE vchannel)
{
    int result;

    result = Mix_HaltChannel(NUM2INT(vchannel));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_expire_channel(VALUE klass, VALUE vchannel, VALUE vticks)
{
    int result;

    result = Mix_ExpireChannel(NUM2INT(vchannel), NUM2INT(vticks));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_fade_out_channel(VALUE klass, VALUE vchannel, VALUE vms)
{
    int result;

    result = Mix_FadeOutChannel(NUM2INT(vchannel), NUM2INT(vms));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_playing(VALUE klass, VALUE vchannel)
{
    int result;

    result = Mix_Playing(NUM2INT(vchannel));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_paused(VALUE klass, VALUE vchannel)
{
    int result;

    result = Mix_Paused(NUM2INT(vchannel));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_fading_channel(VALUE klass, VALUE vchannel)
{
    Mix_Fading result;

    result = Mix_FadingChannel(NUM2INT(vchannel));
    return INT2NUM(result);
}


static VALUE sdl2r_mix_get_chunk(VALUE klass, VALUE vchannel)
{
    Mix_Chunk *chunk;
    VALUE vchunk;

    chunk = Mix_GetChunk(NUM2INT(vchannel));
    if (!chunk) {
        return Qnil;
    }
    vchunk = sdl2r_get_hash(sdl2r_chunk_hash, (HASHKEY)chunk);

    return vchunk;
}




// Music
static VALUE sdl2r_mix_load_mus(VALUE klass, VALUE vfilename)
{
    VALUE vmusic = sdl2r_music_alloc(cMusic);
    struct SDL2RMusic *mus = SDL2R_GET_STRUCT(Music, vmusic);

    Check_Type(vfilename, T_STRING);
    SDL2R_RETRY(mus->music = Mix_LoadMUS(RSTRING_PTR(vfilename)));
    if (!mus->music) {
        rb_raise(eSDLError, Mix_GetError());
    }
    sdl2r_put_hash(sdl2r_music_hash, (HASHKEY)mus->music, vmusic);

    return vmusic;
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
#define SDL2R_DEFINE_SINGLETON_METHOD_MIX(name, arity) rb_define_singleton_method(mMixer, #name, sdl2r_mix_##name, arity)

    SDL2R_DEFINE_SINGLETON_METHOD_MIX(open_audio, 4);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(close_audio, 0);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(query_spec, 0);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(linked_version, 0);

    // Chunk
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(load_wav, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(load_wav_rw, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(play_channel, 3);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(play_channel_timed, 4);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(fade_in_channel, 4);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(fade_in_channel_timed, 5);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(pause, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(resume, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(halt_channel, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(expire_channel, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(fade_out_channel, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(free_chunk, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(volume_chunk, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(allocate_channels, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(volume, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(playing, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(paused, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(fading_channel, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(get_chunk, 1);

    // Music
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(load_mus, 1);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(play_music, 2);
    SDL2R_DEFINE_SINGLETON_METHOD_MIX(free_music, 1);

    // SDL::Mix::Chunk class
    cChunk = rb_define_class_under(mMixer, "Chunk", rb_cObject);
    rb_define_alloc_func(cChunk, sdl2r_chunk_alloc);

    rb_define_method(cChunk, "dispose", sdl2r_chunk_im_dispose, 0);
    rb_define_method(cChunk, "disposed?", sdl2r_chunk_im_get_disposed, 0);

    // SDL::Mix::Music class
    cMusic = rb_define_class_under(mMixer, "Music", rb_cObject);
    rb_define_alloc_func(cMusic, sdl2r_music_alloc);

    rb_define_method(cMusic, "dispose", sdl2r_music_im_dispose, 0);
    rb_define_method(cMusic, "disposed?", sdl2r_music_im_get_disposed, 0);

    // ChannelFinishedEvent
    ChannelFinishedEventID = SDL_RegisterEvents(1);
    Mix_ChannelFinished(sdl2r_mix_ChannelFinishedCallBack);

    // Constants
    rb_define_const(mSDL, "MIXER_CHANNELFINISHED", UINT2NUM(ChannelFinishedEventID));
    rb_define_const(mSDL, "MIXER_VERSION", sdl2r_macro_MIXER_VERSION(mSDL));

    SDL2R_DEFINE_CONST(MIXER_MAJOR_VERSION);
    SDL2R_DEFINE_CONST(MIXER_MINOR_VERSION);
    SDL2R_DEFINE_CONST(MIXER_PATCHLEVEL);

#define SDL2R_DEFINE_CONST_MIX(t) rb_define_const(mMixer, #t, INT2NUM(MIX_##t))
    SDL2R_DEFINE_CONST_MIX(DEFAULT_FREQUENCY);
    SDL2R_DEFINE_CONST_MIX(CHANNELS);
    SDL2R_DEFINE_CONST_MIX(DEFAULT_FORMAT);
    SDL2R_DEFINE_CONST_MIX(DEFAULT_CHANNELS);
    SDL2R_DEFINE_CONST_MIX(MAX_VOLUME);

    SDL2R_DEFINE_ENUM(EnumAudioFormat);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_U8);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_S8);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_U16LSB);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_S16LSB);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_U16MSB);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_S16MSB);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_U16);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_S16);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_U16SYS);
    SDL2R_DEFINE_ENUM_VALUE_N(EnumAudioFormat, AUDIO_S16SYS);

#define SDL2R_DEFINE_ENUM_VALUE_MUS(t, v) {rb_hash_aset(sdl2r_##t, INT2NUM(v), rb_str_new(#v, strlen(#v)));rb_define_const(mMixer, #v, INT2NUM(v));}
    SDL2R_DEFINE_ENUM(EnumMusicType);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_NONE);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_CMD);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_WAV);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_MOD);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_MID);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_OGG);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_MP3);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_MP3_MAD);
    SDL2R_DEFINE_ENUM_VALUE_MUS(EnumMusicType, MUS_FLAC);

#define SDL2R_DEFINE_ENUM_VALUE_MIX(t, v) {rb_hash_aset(sdl2r_##t, INT2NUM(MIX_##v), rb_str_new(#v, strlen(#v)));rb_define_const(mMixer, #v, INT2NUM(MIX_##v));}
    SDL2R_DEFINE_ENUM(EnumFading);
    SDL2R_DEFINE_ENUM_VALUE_MIX(EnumFading, NO_FADING);
    SDL2R_DEFINE_ENUM_VALUE_MIX(EnumFading, FADING_OUT);
    SDL2R_DEFINE_ENUM_VALUE_MIX(EnumFading, FADING_IN);

    sdl2r_chunk_hash = sdl2r_hash_alloc(8);
    sdl2r_music_hash = sdl2r_hash_alloc(8);
}
