#define SDL2RRWOPS
#include "sdl2r.h"
#include "sdl2r_rwops.h"

VALUE cRWops;

static void sdl2r_rwops_mark(void *ptr);
static void sdl2r_rwops_free(void *ptr);

const rb_data_type_t sdl2r_rwops_data_type = {
    "RWops",
    {
    sdl2r_rwops_mark,
    sdl2r_rwops_free,
    0,
    },
};


static void sdl2r_rwops_mark(void *ptr)
{
    struct SDL2RRWops *rw = ptr;
    rb_gc_mark(rw->vstr);
}


static void sdl2r_rwops_free(void *ptr)
{
    struct SDL2RRWops *rw = ptr;
    if (rw->rwops) {
        SDL_RWclose(rw->rwops);
        rw->rwops = 0;
        rw->vstr = Qnil;
    }
    xfree(rw);
}


VALUE sdl2r_rwops_alloc(VALUE klass)
{
    struct SDL2RRWops *rw;
    VALUE vrwops = TypedData_Make_Struct(klass, struct SDL2RRWops, &sdl2r_rwops_data_type, rw);
    rw->rwops = 0;
    rw->vstr = Qnil;
    return vrwops;
}


static VALUE sdl2r_rwops_get_destroyed(VALUE self)
{
    struct SDL2RRWops *rw = SDL2R_GET_STRUCT(RWops, self);
    return rw->rwops ? Qfalse : Qtrue;
}


static VALUE sdl2r_rw_from_file(VALUE klass, VALUE vfilename, VALUE vmode)
{
    VALUE vrwops = sdl2r_rwops_alloc(cRWops);
    struct SDL2RRWops *rw = SDL2R_GET_STRUCT(RWops, vrwops);

    Check_Type(vfilename, T_STRING);
    if (rb_enc_get_index(vfilename) != 0) {
        vfilename = rb_str_export_to_enc(vfilename, g_enc_utf8);
    }
    Check_Type(vmode, T_STRING);
    if (rb_enc_get_index(vmode) != 0) {
        vmode = rb_str_export_to_enc(vmode, g_enc_utf8);
    }

    rw->rwops = SDL_RWFromFile(RSTRING_PTR(vfilename), RSTRING_PTR(vmode));
    if (!rw->rwops) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrwops;
}


static VALUE sdl2r_rw_from_mem(VALUE klass, VALUE vstr, VALUE vsize)
{
    VALUE vrwops = sdl2r_rwops_alloc(cRWops);
    struct SDL2RRWops *rw = SDL2R_GET_STRUCT(RWops, vrwops);

    rw->rwops = SDL_RWFromMem(RSTRING_PTR(vstr), NUM2INT(vsize));
    if (!rw->rwops) {
        rb_raise(eSDLError, SDL_GetError());
    }
    rw->vstr = vstr;

    return vrwops;
}


static VALUE sdl2r_rw_close(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (SDL_RWclose(rw->rwops)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    rw->rwops = 0;
    rw->vstr = Qnil;

    return vrwops;
}


static VALUE sdl2r_rw_read(VALUE klass)
{
}
static VALUE sdl2r_rw_seek(VALUE klass)
{
}
static VALUE sdl2r_rw_size(VALUE klass)
{
}
static VALUE sdl2r_rw_tell(VALUE klass)
{
}
static VALUE sdl2r_rw_write(VALUE klass)
{
}
static VALUE sdl2r_read_be_16(VALUE klass)
{
}
static VALUE sdl2r_read_be_32(VALUE klass)
{
}
static VALUE sdl2r_read_be_64(VALUE klass)
{
}
static VALUE sdl2r_read_le_16(VALUE klass)
{
}
static VALUE sdl2r_read_le_32(VALUE klass)
{
}
static VALUE sdl2r_read_le_64(VALUE klass)
{
}
static VALUE sdl2r_write_be_16(VALUE klass)
{
}
static VALUE sdl2r_write_be_32(VALUE klass)
{
}
static VALUE sdl2r_write_be_64(VALUE klass)
{
}
static VALUE sdl2r_write_le_16(VALUE klass)
{
}
static VALUE sdl2r_write_le_32(VALUE klass)
{
}
static VALUE sdl2r_write_le_64(VALUE klass)
{
}


void Init_sdl2r_rwops(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "rw_from_file", sdl2r_rw_from_file, 2);
    rb_define_singleton_method(mSDL, "rw_from_mem", sdl2r_rw_from_mem, 2);
    rb_define_singleton_method(mSDL, "rw_close", sdl2r_rw_close, 1);
    rb_define_singleton_method(mSDL, "rw_read", sdl2r_rw_read, 4);
    rb_define_singleton_method(mSDL, "rw_seek", sdl2r_rw_seek, 3);
    rb_define_singleton_method(mSDL, "rw_size", sdl2r_rw_size, 1);
    rb_define_singleton_method(mSDL, "rw_tell", sdl2r_rw_tell, 1);
    rb_define_singleton_method(mSDL, "rw_write", sdl2r_rw_write, 4);
    rb_define_singleton_method(mSDL, "read_be_16", sdl2r_read_be_16, 1);
    rb_define_singleton_method(mSDL, "read_be_32", sdl2r_read_be_32, 1);
    rb_define_singleton_method(mSDL, "read_be_64", sdl2r_read_be_64, 1);
    rb_define_singleton_method(mSDL, "read_le_16", sdl2r_read_le_16, 2);
    rb_define_singleton_method(mSDL, "read_le_32", sdl2r_read_le_32, 2);
    rb_define_singleton_method(mSDL, "read_le_64", sdl2r_read_le_64, 2);
    rb_define_singleton_method(mSDL, "write_be_16", sdl2r_write_be_16, 1);
    rb_define_singleton_method(mSDL, "write_be_32", sdl2r_write_be_32, 1);
    rb_define_singleton_method(mSDL, "write_be_64", sdl2r_write_be_64, 1);
    rb_define_singleton_method(mSDL, "write_le_16", sdl2r_write_le_16, 2);
    rb_define_singleton_method(mSDL, "write_le_32", sdl2r_write_le_32, 2);
    rb_define_singleton_method(mSDL, "write_le_64", sdl2r_write_le_64, 2);

    // SDL::RWops class
    cRWops = rb_define_class_under(mSDL, "RWops", rb_cObject);
    rb_define_alloc_func(cRWops, sdl2r_rwops_alloc);

    rb_define_method(cRWops, "destroyed?", sdl2r_rwops_get_destroyed, 0);
}

