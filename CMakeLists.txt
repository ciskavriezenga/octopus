cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")

# Create the project
project(Octopus CXX)

# Add global definitions and include directories
add_definitions(-std=c++14 -Wall)
include_directories(/usr/local/include)

# Setup the common headers and sources
set(HEADERS arithmetic.hpp binary_operation.hpp clock.hpp division.hpp fold.hpp join.hpp modulo.hpp negation.hpp octopus.hpp polymorphic_value.hpp product.hpp sieve.hpp signal.hpp signal_base.hpp sink.hpp split.hpp subtraction.hpp sum.hpp unary_operation.hpp value.hpp)
set(SOURCES clock.cpp signal_base.cpp sink.cpp)

source_group(\\ FILES ${HEADERS} ${SOURCES})
install(FILES ${HEADERS} DESTINATION include/octopus)

# Create the target
add_library(Octopus ${HEADERS} ${SOURCES})
install(TARGETS Octopus DESTINATION lib)

if (BUILD_SHARED_LIBS)
	set_target_properties(Octopus PROPERTIES OUTPUT_NAME octopus)
else (BUILD_SHARED_LIBS)
	set_target_properties(Octopus PROPERTIES OUTPUT_NAME octopus_static)
endif (BUILD_SHARED_LIBS)