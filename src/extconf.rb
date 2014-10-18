require "mkmf"

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

