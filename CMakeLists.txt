cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE APP_SRC_FILES
    ${CMAKE_SOURCE_DIR}/almond/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
list(APPEND APP_SRC_FILES ${CMAKE_SOURCE_DIR}/main.cpp)

add_executable(VisualizerLinearTransforms ${APP_SRC_FILES})

target_compile_features(VisualizerLinearTransforms PRIVATE cxx_std_17)

target_include_directories(VisualizerLinearTransforms PRIVATE
    ${CMAKE_SOURCE_DIR}/almond
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(VisualizerLinearTransforms
    PRIVATE
        sfml-graphics
        GLEW::GLEW
        OpenGL::GL
        glm::glm
)

add_custom_command(TARGET VisualizerLinearTransforms PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMENT "Copying assets to output directory"
)

set_target_properties(VisualizerLinearTransforms PROPERTIES OUTPUT_NAME main)
