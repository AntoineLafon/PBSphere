cmake_minimum_required(VERSION 3.1)

project(Demo)

#set(CMAKE_PREFIX_PATH "~/Desktop/radium/Radium-Engine/Bundle-GNU/lib/cmake/Radium")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

find_package(Radium REQUIRED Core Engine Gui IO PluginBase)
find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Particle.cpp Particle.h ParticleManager.cpp ParticleManager.h Constraint.cpp Constraint.h KDTree.cpp KDTree.h)

if(Radium_FOUND)
    target_link_libraries(Demo PUBLIC Radium::Core Radium::Engine Radium::Gui
            Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Xml)
else()
    message(FATAL_ERROR "Radium engine not found")
endif()
