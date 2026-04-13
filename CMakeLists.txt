cmake_minimum_required(VERSION 3.10)
project(rmq_experiments)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_CXX_STANDARD 20)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Information")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -DINFO")
endif()

include_directories(competitors)

file(GLOB HYPERRMQ_SOURCES ${CMAKE_SOURCE_DIR}/competitors/hyperrmq/src/*.cpp)
add_library(hyperrmq_lib STATIC ${HYPERRMQ_SOURCES})
target_include_directories(hyperrmq_lib PUBLIC ${CMAKE_SOURCE_DIR}/competitors/hyperrmq/include)

add_library(cct_lib INTERFACE)
target_include_directories(cct_lib INTERFACE
    ${CMAKE_SOURCE_DIR}/competitors/cct/include
    ${CMAKE_SOURCE_DIR}/competitors/cct/sdsl-lite/include
    ${CMAKE_SOURCE_DIR}/competitors/cct/bit_vector/include
    ${CMAKE_SOURCE_DIR}/competitors/cct/bit_vector/_deps/pasta_utils-src/include
    ${CMAKE_SOURCE_DIR}/competitors/cct/bit_vector/_deps/tlx-src
)

add_library(libsais STATIC IMPORTED)
set_target_properties(libsais PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/competitors/cct/libsais/libsais.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/competitors/cct/libsais/include"
)

target_link_libraries(cct_lib INTERFACE libsais)

add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/cxxopts/include)

add_subdirectory(benchmark)
