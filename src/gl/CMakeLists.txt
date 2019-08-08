message(STATUS "Creating target rogl")

set(SOURCE_FILES
    "ApolloSprite.cpp"
    "ApolloSprite.hpp"
    "Effect.cpp"
    "Effect.hpp"
    "ROSprite.cpp"
    "ROSprite.hpp"
    "Sprite.hpp"
    "Texture.cpp"
    "Texture.hpp")

add_library(rogl STATIC ${SOURCE_FILES})
target_link_libraries(rogl roformat)

message(STATUS "Creating target rogl - done")