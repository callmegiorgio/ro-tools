message(STATUS "Creating target roformat")

set(SOURCE_FILES
    "Act.cpp"
    "Act.hpp"
    "Pal.cpp"
    "Pal.hpp"
    "Spr.cpp"
    "Spr.hpp")

add_library(roformat STATIC ${SOURCE_FILES})

message(STATUS "Creating target roformat - done")