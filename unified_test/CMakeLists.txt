cmake_minimum_required(VERSION 3.25)

# Create unified timetest runner
add_executable(unified_timetest
  main.cpp
  timetest_wrappers.cpp
)

# Each wrapper includes just one header file, avoiding conflicts
target_include_directories(unified_timetest PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}  # For timetest_wrappers.h
)

# Include directories for each wrapper file separately
target_include_directories(unified_timetest PRIVATE
  ${CMAKE_SOURCE_DIR}/MSVC_CH_5/include
)

target_include_directories(unified_timetest PRIVATE
  ${CMAKE_SOURCE_DIR}/MSVC_RH_4/include
)

target_include_directories(unified_timetest PRIVATE
  ${CMAKE_SOURCE_DIR}/MSVC_RH_5/include
)

target_include_directories(unified_timetest PRIVATE
  ${CMAKE_SOURCE_DIR}/MSVC_SP_4/include
)

target_include_directories(unified_timetest PRIVATE
  ${CMAKE_SOURCE_DIR}/MSVC_SP_5/include
)

# Common include directory
target_include_directories(unified_timetest PRIVATE
  ${CMAKE_SOURCE_DIR}/common/include
)

# Link against all the timetest libraries and timetest executables
target_link_libraries(unified_timetest
  PRIVATE
    MSVC_CH_5_lib
    MSVC_RH_4_lib
    MSVC_RH_5_lib
    MSVC_SP_4_lib
    MSVC_SP_5_lib
)