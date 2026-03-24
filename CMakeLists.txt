cmake_minimum_required(VERSION 3.20)

project(active-radar-sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Magnum REQUIRED
    GL
    MeshTools
    Primitives
    Shaders
    Sdl2Application
)

add_executable(active-radar-sim
    src/Entities.cpp
    src/Main.cpp
    src/Physics.cpp
    src/Simulation.cpp
    src/Vizualisation.cpp
)

target_link_libraries(active-radar-sim PRIVATE
    Magnum::GL
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::Sdl2Application
)
