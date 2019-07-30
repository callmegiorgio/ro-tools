message(STATUS "Creating target rowindow")

set(SOURCE_FILES
    "glad.c"
    "Key.hpp"
    "KeyEvent.hpp"
    "ModifierKeys.hpp"
    "MouseButton.hpp"
    "MouseButtonEvent.hpp"
    "MouseMotionEvent.hpp"
    "MouseWheelEvent.hpp"
    "Window.cpp"
    "Window.hpp")

add_library(rowindow STATIC ${SOURCE_FILES})

message(STATUS "Creating target rowindow - done")