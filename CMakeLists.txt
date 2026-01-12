cmake_minimum_required(VERSION 3.10) #
project(CppEngine VERSION 1.0)      #

set(SOURCES
    main.cpp
    Engine/GPhysics.cpp
    Engine/GScene.cpp
    Engine/ColliderComp.cpp
    Engine/GObject.cpp
)

add_executable(run ${SOURCES}) #
target_link_libraries(run sfml-graphics sfml-window sfml-system)
