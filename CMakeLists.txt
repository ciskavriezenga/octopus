cmake_minimum_required(VERSION 3.5.1)

project(octopus CXX)

include_directories(/usr/local/include)
add_definitions("-std=c++1z -Wall")

add_library(octopus SHARED "")

# Core
set(HEADERS
    Arithmetic.hpp
    BinaryOperation.hpp
    Clock.hpp
    Division.hpp
    Fold.hpp
    Join.hpp
    Latest.hpp
    Negation.hpp
    Node.hpp
    Product.hpp
	Signal.hpp
	SignalBase.hpp
    Sine.hpp
    Subtraction.hpp
    Sum.hpp
    UnaryOperation.hpp
    Value.hpp)

set(SOURCES
	SignalBase.cpp)

target_sources(octopus PRIVATE ${HEADERS} ${SOURCES})
source_group(\\ FILES ${HEADERS} ${SOURCES})
install (FILES ${HEADERS} DESTINATION include/octopus)
