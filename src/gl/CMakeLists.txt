message(STATUS "Creating target rogl")

set(SOURCE_FILES
    "ApolloSprite.cpp"
    "ApolloSprite.hpp"
    "ROSprite.cpp"
    "ROSprite.hpp"
    "Sprite.hpp"
    "Texture.cpp"
    "Texture.hpp")

add_library(rogl STATIC ${SOURCE_FILES})

message(STATUS "Creating target rogl - done")