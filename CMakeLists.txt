cmake_minimum_required(VERSION 3.5.1)

project(octopus CXX)

include_directories(/usr/local/include)
add_definitions("-std=c++1z -Wall")

add_library(octopus SHARED "")

# Core
set(HEADERS
	arithmetic.hpp
	binary_operation.hpp
	clock.hpp
	division.hpp
	fold.hpp
	join.hpp
	negation.hpp
	octopus.hpp
    polymorphic_value.hpp
	product.hpp
	sieve.hpp
	signal.hpp
	signal_base.hpp
    sink.hpp
	split.hpp
	subtraction.hpp
	sum.hpp
	unary_operation.hpp
	value.hpp)

set(SOURCES
    signal_base.cpp
    sink.cpp)

target_sources(octopus PRIVATE ${HEADERS} ${SOURCES})
source_group(\\ FILES ${HEADERS} ${SOURCES})
install(TARGETS octopus DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/octopus)
