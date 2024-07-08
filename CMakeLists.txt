cmake_minimum_required(VERSION 3.24)
project(Gra_Hexaggon)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

add_executable(Gra_Hexaggon main.cpp GamePanel.cpp GamePanel.cpp Hexagon.cpp Hexagon.h GamePanel.h Move.h Move.cpp)
target_compile_definitions(Gra_Hexaggon PRIVATE SFML_STATIC)
target_link_libraries(Gra_Hexaggon PRIVATE sfml-graphics sfml-audio sfml-network sfml-window sfml-system)