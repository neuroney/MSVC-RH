# First create a library that contains the implementation
add_library(MSVC_RH_4_lib
    src/MSVC_RH_4.cpp
    src/MSVC_RH_4_timetest.cpp
)

target_include_directories(MSVC_RH_4_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MSVC_RH_4_lib
    PUBLIC
        common
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)

# Then create the executables that use the library
add_executable(MSVC_RH_4
    test/test.cpp
    src/MSVC_RH_4_test.cpp
)

target_link_libraries(MSVC_RH_4
    PRIVATE
        MSVC_RH_4_lib
)

# Add time testing executable
add_executable(MSVC_RH_4_timetest
    test/timetest.cpp
    src/MSVC_RH_4_timetest.cpp  # This would be a new file with just main()
)

target_link_libraries(MSVC_RH_4_timetest
    PRIVATE
        MSVC_RH_4_lib
)