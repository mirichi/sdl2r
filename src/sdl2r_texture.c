#include <SDL.h>

#define SDL2RTEXTURE
#include "sdl2r.h"
#include "sdl2r_surface.h"
#include "sdl2r_window.h"
#include "sdl2r_texture.h"

VALUE cTexture;

static void sdl2r_texture_free(void *ptr);
static void sdl2r_texture_mark(void *ptr);

const rb_data_type_t sdl2r_texture_data_type = {
    "Texture",
    {
    sdl2r_texture_mark,
    sdl2r_texture_free,
    0,
    },
};


static void sdl2r_texture_free(void *ptr)
{
    struct SDL2RTexture *tex = ptr;

    if (tex->texture) {
        SDL_DestroyTexture(tex->texture);
    }

    xfree(tex);
}


static void sdl2r_texture_mark(void *ptr)
{
    struct SDL2RTexture *tex = ptr;
    rb_gc_mark(tex->vrenderer);
}


VALUE sdl2r_texture_alloc(VALUE klass)
{
    struct SDL2RTexture *tex;
    VALUE vtex = TypedData_Make_Struct(klass, struct SDL2RTexture, &sdl2r_texture_data_type, tex);
    tex->texture = 0;
    tex->vrenderer = Qnil;
    return vtex;
}


static VALUE sdl2r_query_texture(VALUE klass, VALUE vtexture)
{
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);
    Uint32 format;
    int access, w, h;

    if (SDL_QueryTexture(tex->texture, &format, &access, &w, &h)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return rb_ary_new3(4, INT2NUM(format), INT2NUM(access), INT2NUM(w), INT2NUM(h));
}


static VALUE sdl2r_destroy_texture(VALUE klass, VALUE vtexture)
{
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);

    SDL_DestroyTexture(tex->texture);
    tex->texture = 0;

    return vtexture;
}


static VALUE sdl2r_texture_get_destroyed(VALUE self)
{
    struct SDL2RTexture *tex = SDL2R_GET_STRUCT(Texture, self);
    return tex->texture ? Qfalse : Qtrue;
}


static VALUE sdl2r_set_texture_blend_mode(VALUE klass, VALUE vtexture, VALUE vblendmode)
{
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);

    if (SDL_SetTextureBlendMode(tex->texture, NUM2INT(vblendmode))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vtexture;
}


static VALUE sdl2r_get_texture_blend_mode(VALUE klass, VALUE vtexture)
{
    struct SDL2RTexture *tex = SDL2R_GET_TEXTURE_STRUCT(vtexture);
    SDL_BlendMode bm;

    if (SDL_GetTextureBlendMode(tex->texture, &bm)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return INT2NUM(bm);
}


void Init_sdl2r_texture(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "query_texture", sdl2r_query_texture, 1);
    rb_define_singleton_method(mSDL, "destroy_texture", sdl2r_destroy_texture, 1);
    rb_define_singleton_method(mSDL, "set_texture_blend_mode", sdl2r_set_texture_blend_mode, 2);
    rb_define_singleton_method(mSDL, "get_texture_blend_mode", sdl2r_get_texture_blend_mode, 1);

    // SDL::Texture class
    cTexture = rb_define_class_under(mSDL, "Texture", rb_cObject);
    rb_define_alloc_func(cTexture, sdl2r_texture_alloc);

    rb_define_method(cTexture, "destroyed?", sdl2r_texture_get_destroyed, 0);

    // Constants
}


