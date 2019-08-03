message(STATUS "Creating target roformat")

set(SOURCE_FILES
    "Act.cpp"
    "Act.hpp"
    "Pal.cpp"
    "Pal.hpp"
    "Palette.hpp"
    "Spr.cpp"
    "Spr.hpp"
    "Sprite.cpp"
    "Sprite.hpp")

add_library(roformat STATIC ${SOURCE_FILES})

message(STATUS "Creating target roformat - done")