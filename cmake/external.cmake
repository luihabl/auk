include(cmake/cpm.cmake)

# SDL2
CPMAddPackage(
  NAME sdl2
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG release-2.26.x
  OPTIONS 
    "SDL_TEST_ENABLED_BY_DEFAULT Off" 
    "SDL_TEST Off"
    "SDL_SHARED Off"
)

# rect_pack.h
CPMAddPackage("gh:luihabl/rect_pack.h#main")
