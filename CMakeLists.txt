cmake_minimum_required(VERSION 3.5.1)

project(octopus CXX)

include_directories(/usr/local/include)
add_definitions("-std=c++1z -Wall")

add_library(octopus SHARED "")

# Core
set(HEADERS
    Clock.hpp
    Fold.hpp
    Latest.hpp
    Node.hpp
	Signal.hpp
	SignalBase.hpp
    Sine.hpp
    Sum.hpp
    Value.hpp)

set(SOURCES
	SignalBase.cpp)

target_sources(octopus PRIVATE ${HEADERS} ${SOURCES})
source_group(\\ FILES ${HEADERS} ${SOURCES})
install (FILES ${HEADERS} DESTINATION include/octopus)
