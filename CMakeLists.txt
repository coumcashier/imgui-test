cmake_minimum_required(VERSION 3.0)
project(example)
set(CMAKE_CXX_STANDARD 20)


file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h)

add_executable( main
        main.cpp
        my_widgets.cpp
        libs/imgui/backends/imgui_impl_sdl3.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
        ${HEADER_FILES}
 )

add_library( imgui 
        libs/imgui/imgui.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/misc/cpp/imgui_stdlib.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_demo.cpp
 )

 target_include_directories(
    imgui
    PUBLIC
    ./libs/imgui
 )

target_include_directories(
    main
    PUBLIC
    libs/imgui/misc/cpp
    libs/sdl3/include
    libs/imgui
    libs/imgui/backends
    .
    )

find_package(OpenGL REQUIRED)


target_link_directories(main PRIVATE libs/sdl3/lib)
target_link_libraries(main SDL3 imgui OpenGL::GL)


add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/libs/sdl3/bin"
        $<TARGET_FILE_DIR:main>)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:main>/data)