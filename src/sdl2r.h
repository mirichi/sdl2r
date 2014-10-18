#include "ruby.h"
#include "ruby/encoding.h"

#ifdef SDL2R
VALUE mSDL;
VALUE eSDLError;
VALUE eSDL2RError;
rb_encoding *g_enc_utf8;
#else
extern VALUE mSDL;
extern VALUE eSDLError;
extern VALUE eSDL2RError;
extern rb_encoding *g_enc_utf8;
#endif

#define SDL2R_GET_STRUCT(name, obj) ((struct SDL2R##name *)DATA_PTR(obj))
#define SDL2R_RETRY(test) {int retry_flag = 0;do {\
                            if (!(test) && !retry_flag) {rb_gc_start();retry_flag = 1;}else{retry_flag = 0;}\
}                          while(retry_flag);}
