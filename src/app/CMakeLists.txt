set(UTIL_HEADERS
    "../util/Buffer.hpp"
    "../util/Color.hpp"
    "../util/filehandler.hpp"
    "../util/InvalidFile.hpp")

function(console_app app_name)
    message(STATUS "Creating target ${app_name}")
    add_executable(${app_name} ${app_name}.cpp ${UTIL_HEADERS})
    target_link_libraries(${app_name} roformat)
    message(STATUS "Creating target ${app_name} - done")
endfunction(console_app)

function(window_app app_name)
    message(STATUS "Creating target ${app_name}")
    add_executable(${app_name} ${app_name}.cpp ${UTIL_HEADERS})
    target_link_libraries(${app_name} roformat rowindow rogl glfw GL dl)
    message(STATUS "Creating target ${app_name} - done")
endfunction(window_app)

console_app(01_pal_info)
window_app(02_pal_window)
console_app(03_spr_info)
window_app(04_spr_window)
console_app(05_act_info)
window_app(06_act_viewer)
window_app(07_multi_act_viewer)