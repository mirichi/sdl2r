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


static VALUE sdl2r_rwops_get_disposed(VALUE self)
{
    struct SDL2RRWops *rw = SDL2R_GET_STRUCT(RWops, self);
    return rw->rwops ? Qfalse : Qtrue;
}


static VALUE sdl2r_rw_from_file(VALUE klass, VALUE vfilename, VALUE vmode)
{
    VALUE vrwops = sdl2r_rwops_alloc(cRWops);
    struct SDL2RRWops *rw = SDL2R_GET_STRUCT(RWops, vrwops);

    rw->rwops = SDL_RWFromFile(SDL2R_TO_UTF8_PTR(vfilename), SDL2R_TO_UTF8_PTR(vmode));
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


static VALUE sdl2r_rw_read(VALUE klass, VALUE vrwops, VALUE vsize, VALUE vmaxnum)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    char *buf = ALLOCA_N(char, NUM2UINT(vsize) * NUM2UINT(vmaxnum));
    size_t result;

    result = SDL_RWread(rw->rwops, buf, NUM2UINT(vsize), NUM2UINT(vmaxnum));
    if (result == 0) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return rb_str_new(buf, result * NUM2UINT(vsize));
}


static VALUE sdl2r_rw_seek(VALUE klass, VALUE vrwops, VALUE voffset, VALUE vwhence)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    LONG_LONG result;

    result = SDL_RWseek(rw->rwops, NUM2LL(voffset), NUM2INT(vwhence));
    if (result == -1) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return LL2NUM(result);
}


static VALUE sdl2r_rw_size(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    LONG_LONG result;

    result = SDL_RWsize(rw->rwops);
     if (result == -1) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return LL2NUM(result);
}


static VALUE sdl2r_rw_tell(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    LONG_LONG result;

    result = SDL_RWtell(rw->rwops);
     if (result == -1) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return LL2NUM(result);
}


static VALUE sdl2r_rw_write(VALUE klass, VALUE vrwops, VALUE vstr, VALUE vsize, VALUE vnum)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    size_t result;

    result = SDL_RWwrite(rw->rwops, RSTRING_PTR(vstr), NUM2UINT(vsize), NUM2UINT(vnum));
    if (result < NUM2UINT(vnum)) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vrwops;
}


static VALUE sdl2r_read_be_16(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    return INT2NUM(SDL_ReadBE16(rw->rwops));
}


static VALUE sdl2r_read_be_32(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    return INT2NUM(SDL_ReadBE32(rw->rwops));
}


static VALUE sdl2r_read_be_64(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    return LL2NUM(SDL_ReadBE64(rw->rwops));
}


static VALUE sdl2r_read_le_16(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    return INT2NUM(SDL_ReadLE16(rw->rwops));
}


static VALUE sdl2r_read_le_32(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    return INT2NUM(SDL_ReadLE32(rw->rwops));
}


static VALUE sdl2r_read_le_64(VALUE klass, VALUE vrwops)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);
    return LL2NUM(SDL_ReadLE64(rw->rwops));
}


static VALUE sdl2r_write_be_16(VALUE klass, VALUE vrwops, VALUE vdata)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (!SDL_WriteBE16(rw->rwops, NUM2INT(vdata))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vdata;
}


static VALUE sdl2r_write_be_32(VALUE klass, VALUE vrwops, VALUE vdata)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (!SDL_WriteBE32(rw->rwops, NUM2INT(vdata))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vdata;
}


static VALUE sdl2r_write_be_64(VALUE klass, VALUE vrwops, VALUE vdata)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (!SDL_WriteBE64(rw->rwops, NUM2LL(vdata))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vdata;
}


static VALUE sdl2r_write_le_16(VALUE klass, VALUE vrwops, VALUE vdata)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (!SDL_WriteLE16(rw->rwops, NUM2INT(vdata))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vdata;
}


static VALUE sdl2r_write_le_32(VALUE klass, VALUE vrwops, VALUE vdata)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (!SDL_WriteLE32(rw->rwops, NUM2INT(vdata))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vdata;
}


static VALUE sdl2r_write_le_64(VALUE klass, VALUE vrwops, VALUE vdata)
{
    struct SDL2RRWops *rw = SDL2R_GET_RWOPS_STRUCT(vrwops);

    if (!SDL_WriteLE64(rw->rwops, NUM2LL(vdata))) {
        rb_raise(eSDLError, SDL_GetError());
    }

    return vdata;
}


void Init_sdl2r_rwops(void)
{
    // SDL module methods
    rb_define_singleton_method(mSDL, "rw_from_file", sdl2r_rw_from_file, 2);
    rb_define_singleton_method(mSDL, "rw_from_mem", sdl2r_rw_from_mem, 2);
    rb_define_singleton_method(mSDL, "rw_close", sdl2r_rw_close, 1);
    rb_define_singleton_method(mSDL, "rw_read", sdl2r_rw_read, 3);
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

    rb_define_method(cRWops, "disposed?", sdl2r_rwops_get_disposed, 0);

    // Constants
    rb_define_const(mSDL, "RW_SEEK_SET", INT2FIX(RW_SEEK_SET));
    rb_define_const(mSDL, "RW_SEEK_CUR", INT2FIX(RW_SEEK_CUR));
    rb_define_const(mSDL, "RW_SEEK_END", INT2FIX(RW_SEEK_END));
}


