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
	Negation.hpp
	Octopus.hpp
	Product.hpp
	Sieve.hpp
	Signal.hpp
	SignalBase.hpp
    Sink.hpp
	Split.hpp
	Subtraction.hpp
	Sum.hpp
	UnaryOperation.hpp
	Value.hpp)

set(SOURCES
    SignalBase.cpp
    Sink.cpp)

target_sources(octopus PRIVATE ${HEADERS} ${SOURCES})
source_group(\\ FILES ${HEADERS} ${SOURCES})
install(TARGETS octopus DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/octopus)
