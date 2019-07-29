set(UTIL_HEADERS
    "../util/Buffer.hpp"
    "../util/Color.hpp"
    "../util/filehandler.hpp"
    "../util/InvalidFile.hpp"
    "../util/Texture.hpp")

function(console_app app_name)
    message(STATUS "Creating target ${app_name}")
    add_executable(${app_name} ${app_name}.cpp ${UTIL_HEADERS})
    target_link_libraries(${app_name} resource)
    message(STATUS "Creating target ${app_name} - done")
endfunction(console_app)

function(window_app app_name)
    message(STATUS "Creating target ${app_name}")
    add_executable(${app_name} ${app_name}.cpp ${UTIL_HEADERS})
    target_link_libraries(${app_name} resource window glfw GL dl)
    message(STATUS "Creating target ${app_name} - done")
endfunction(window_app)

console_app(01_pal_info)
window_app(02_pal_window)
console_app(03_spr_info)
window_app(04_spr_window)
console_app(05_act_info)