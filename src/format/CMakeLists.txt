message(STATUS "Creating target roformat")

set(SOURCE_FILES
    "Act.cpp"
    "Act.hpp"
    "Image.cpp"
    "Image.hpp"
    "Pal.cpp"
    "Pal.hpp"
    "Spr.cpp"
    "Spr.hpp"
    "Sprite.cpp"
    "Sprite.hpp"
    "Str.cpp"
    "Str.hpp")

add_library(roformat STATIC ${SOURCE_FILES})
target_link_libraries(roformat freeimage)

message(STATUS "Creating target roformat - done")