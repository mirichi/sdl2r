require "mkmf"

if find_executable('sdl2-config')
  sdl2_config = with_config('sdl2-config', 'sdl2-config')
  $CPPFLAGS += " " + `#{sdl2_config} --cflags`.chomp
  $LOCAL_LIBS += " " + `#{sdl2_config} --libs`.chomp
end

SYSTEM_LIBRARIES = [
  "SDL2",
  "SDL2main",
  "SDL2_image",
  "SDL2_mixer",
  "SDL2_ttf",
]
SYSTEM_LIBRARIES.each do |lib|
  have_library(lib)
end

have_header("sdl.h")
have_header("sdl_image.h")
have_header("sdl_mixer.h")
have_header("sdl_ttf.h")


create_makefile("sdl2r")
