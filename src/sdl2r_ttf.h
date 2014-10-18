#include <SDL_ttf.h>

struct SDL2RFont {
    TTF_Font *font;
};

void Init_sdl2r_ttf(void);
VALUE sdl2r_font_alloc(VALUE klass);

#ifndef SDL2RTTF
extern VALUE mTTF;
extern const rb_data_type_t sdl2r_font_data_type;
extern VALUE cFont;
#endif

#define SDL2R_GET_FONT_STRUCT(obj) (\
        (RB_TYPE_P(obj, T_DATA) && RTYPEDDATA_TYPE(obj) == &sdl2r_font_data_type) ?\
            ((SDL2R_GET_STRUCT(Font, obj)->font == NULL) ?\
                rb_raise(eSDL2RError, "destroyed Font object"), SDL2R_GET_STRUCT(Font, obj)\
              : SDL2R_GET_STRUCT(Font, obj)\
            )\
          : rb_raise(rb_eArgError, "wrong argument type %s (expected SDL::TTF::Font)", rb_obj_classname(obj)), SDL2R_GET_STRUCT(Font, obj)\
    )
