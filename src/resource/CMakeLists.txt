message(STATUS "Creating target resource")

set(SOURCE_FILES
    "Act.cpp"
    "Act.hpp"
    "Pal.cpp"
    "Pal.hpp"
    "Spr.cpp"
    "Spr.hpp")

add_library(resource STATIC ${SOURCE_FILES})

message(STATUS "Creating target resource - done")