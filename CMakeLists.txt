cmake_minimum_required(VERSION 3.28)
project(SnakeSFML LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(snake-sfml src/main.cpp src/snake.cpp src/map.cpp)
target_compile_features(snake-sfml PRIVATE cxx_std_17)
target_compile_definitions(snake-sfml PRIVATE SFML_STATIC)
target_link_libraries(snake-sfml PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
    freetype
    opengl32
    winmm
    gdi32
    ws2_32
)

add_custom_command(TARGET snake-sfml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:snake-sfml>/images
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:snake-sfml>/fonts)
