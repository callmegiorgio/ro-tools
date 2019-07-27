cmake_minimum_required(VERSION 3.9)
project(ROTesting)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(3rdparty)
add_subdirectory(window)

function(console_app app_name)
    set(SOURCE_FILES
        "Act.cpp"
        "Act.hpp"
        "Buffer.hpp"
        "Color.hpp"
        "filereader.hpp"
        "InvalidFile.hpp"
        "Pal.cpp"
        "Pal.hpp"
        "Spr.cpp"
        "Spr.hpp"
        "Texture.hpp"
    )

    add_executable(${app_name} ${app_name}.cpp ${SOURCE_FILES})
endfunction(console_app)

function(window_app app_name)
    console_app(${app_name})
    target_link_libraries(${app_name} window glfw GL dl)
endfunction(window_app)

console_app(01_pal_info)
window_app(02_pal_window)
console_app(03_spr_info)
window_app(04_spr_window)
console_app(05_act_info)