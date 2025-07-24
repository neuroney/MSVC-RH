# First create a library that contains the implementation
add_library(MSVC_CH_5_lib
    src/MSVC_CH_5.cpp
    src/MSVC_CH_5_timetest.cpp
)

target_include_directories(MSVC_CH_5_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MSVC_CH_5_lib
    PUBLIC
        common
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)

# Then create the executables that use the library
add_executable(MSVC_CH_5
    test/test.cpp
    src/MSVC_CH_5_test.cpp
)

target_link_libraries(MSVC_CH_5
    PRIVATE
        MSVC_CH_5_lib
)

# Add time testing executable
add_executable(MSVC_CH_5_timetest
    test/timetest.cpp
    src/MSVC_CH_5_timetest.cpp  # This would be a new file with just main()
)

target_link_libraries(MSVC_CH_5_timetest
    PRIVATE
        MSVC_CH_5_lib
)