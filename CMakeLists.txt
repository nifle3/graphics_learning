cmake_minimum_required(VERSION 3.15)
project(BasicGraphics C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4>=4.0)
pkg_check_modules(EPOXY REQUIRED IMPORTED_TARGET epoxy)
pkg_check_modules(JSONC REQUIRED IMPORTED_TARGET json-c)

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/headers
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    OpenGL::GL
    PkgConfig::GTK4
    PkgConfig::EPOXY
    PkgConfig::JSONC
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/ui
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/styles
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/ui ${PROJECT_SOURCE_DIR}/bin/ui
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/styles ${PROJECT_SOURCE_DIR}/bin/styles
)

add_dependencies(${PROJECT_NAME} copy_resources)
