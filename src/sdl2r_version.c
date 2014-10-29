#define SDL2RVERSION
#include "sdl2r.h"
#include "sdl2r_version.h"
#include "sdl_revision.h"

VALUE cVersion;

VALUE sdl2r_get_revision(VALUE klass)
{
    const char *result = SDL_GetRevision();
    return rb_enc_str_new(result, strlen(result), g_enc_utf8);
}


VALUE sdl2r_get_revisionNumber(VALUE klass)
{
    return INT2NUM(SDL_GetRevisionNumber());
}


VALUE sdl2r_get_version(VALUE klass)
{
    SDL_version v;
    VALUE ary[5];

    SDL_GetVersion(&v);
    ary[0] = INT2NUM(v.major);
    ary[1] = INT2NUM(v.minor);
    ary[2] = INT2NUM(v.patch);

    return rb_class_new_instance(3, ary, cVersion);
}


VALUE sdl2r_macro_VERSION(VALUE klass)
{
    SDL_version v;
    VALUE ary[5];

    SDL_VERSION(&v);
    ary[0] = INT2NUM(v.major);
    ary[1] = INT2NUM(v.minor);
    ary[2] = INT2NUM(v.patch);

    return rb_class_new_instance(3, ary, cVersion);
}


VALUE sdl2r_macro_versionnum(VALUE klass, VALUE vx, VALUE vy, VALUE vz)
{
    return INT2NUM(SDL_VERSIONNUM(NUM2INT(vx), NUM2INT(vy), NUM2INT(vz)));
}


VALUE sdl2r_macro_version_atleast(VALUE klass, VALUE vx, VALUE vy, VALUE vz)
{
    return SDL_VERSION_ATLEAST(NUM2INT(vx), NUM2INT(vy), NUM2INT(vz)) ? Qtrue : Qfalse;
}


void Init_sdl2r_version(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "get_revision", sdl2r_get_revision, 0);
    rb_define_singleton_method(mSDL, "get_revisionNumber", sdl2r_get_revisionNumber, 0);
    rb_define_singleton_method(mSDL, "get_version", sdl2r_get_version, 0);

    // Version struct
    cVersion = rb_struct_define(NULL, "major", "minor", "patch", NULL);
    rb_define_const(mSDL, "Version", cVersion);

    // Constants
    rb_define_const(mSDL, "COMPILEDVERSION", INT2FIX(SDL_COMPILEDVERSION));
    rb_define_const(mSDL, "REVISION", rb_enc_str_new(SDL_REVISION, strlen(SDL_REVISION), g_enc_utf8));
    rb_define_const(mSDL, "VERSION", sdl2r_macro_VERSION(mSDL));

    // SDL macro
    rb_define_singleton_method(mSDL, "VERSIONNUM", sdl2r_macro_versionnum, 3);
    rb_define_singleton_method(mSDL, "VERSION_ATLEAST", sdl2r_macro_version_atleast, 3);

}
