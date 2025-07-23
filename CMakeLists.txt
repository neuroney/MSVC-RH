cmake_minimum_required(VERSION 3.25)
project(MSVC VERSION 0.1.0)

# Global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(NTL REQUIRED)
find_package(GMP REQUIRED)

include_directories(${NTL_INCLUDE_PATHS} ${GMP_INCLUDE_PATHS})


# Add common library first
add_subdirectory(common)

# Add all subprojects
add_subdirectory(MSVC_SP_4)
add_subdirectory(MSVC_SP_5)
add_subdirectory(MSVC_CH_5)
add_subdirectory(MSVC_RH_4)
add_subdirectory(MSVC_RH_5)

