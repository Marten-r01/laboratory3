cmake_minimum_required(VERSION 3.24)
project(astar_qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.10 REQUIRED COMPONENTS Widgets)
add_executable(Laboratory_3_garph main.cpp

        GraphMap.h
        Point.h
        Terrain.h
        Heuristic.h
        AStar.h
        Path.h
        MapFactory.h

        EditorWindow.cpp
        EditorWindow.h
        MapSelectWindow.cpp
        MapSelectWindow.h
        MapWidget.cpp
        MapWidget.h
        main.cpp
)

target_link_libraries(Laboratory_3_garph PRIVATE Qt6::Widgets)