message(STATUS "Creating target window")
file(GLOB WINDOW_HEADERS *.hpp)
file(GLOB WINDOW_SOURCES *.cpp)
set(SOURCE_FILES ${WINDOW_HEADERS} ${WINDOW_SOURCES} "glad.c")

add_library(window STATIC ${SOURCE_FILES})

message(STATUS "Creating target window - done")