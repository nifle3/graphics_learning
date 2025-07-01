cmake_minimum_required(VERSION 3.15)
project(BasicGraphics C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4>=4.0)
pkg_check_modules(EPOXY REQUIRED IMPORTED_TARGET epoxy)

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
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

file(GLOB UI_FILES "${PROJECT_SOURCE_DIR}/ui/*.xml")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ui"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${UI_FILES}
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ui"
)

file(GLOB STYLE_FILES "${PROJECT_SOURCE_DIR}/styles/*.css")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/styles"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${STYLE_FILES}
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/styles"
)
