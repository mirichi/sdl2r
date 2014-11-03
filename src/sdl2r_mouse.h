struct SDL2RCursor {
    SDL_Cursor *cursor;
};

void Init_sdl2r_mouse(void);
VALUE sdl2r_cursor_alloc(VALUE klass);

#ifndef SDL2RMOUSE
extern const rb_data_type_t sdl2r_cursor_data_type;
extern VALUE cCursor;
#endif

#define SDL2R_GET_CURSOR_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_cursor_data_type) ?\
            ((SDL2R_GET_STRUCT(Cursor, obj)->cursor == NULL) ?\
                rb_raise(eSDL2RError, "disposed Cursor object"), SDL2R_GET_STRUCT(Cursor, obj)\
              : SDL2R_GET_STRUCT(Cursor, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::Cursor)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Cursor, obj)\
    )


