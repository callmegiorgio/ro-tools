message(STATUS "Creating target rogl")

set(SOURCE_FILES
    "Sprite.cpp"
    "Sprite.hpp"
    "Texture.cpp"
    "Texture.hpp")

add_library(rogl STATIC ${SOURCE_FILES})

message(STATUS "Creating target rogl - done")