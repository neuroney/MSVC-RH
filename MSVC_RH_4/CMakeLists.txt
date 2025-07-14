add_executable(MSVC_RH_4
    src/MSVC_RH_4.cpp
    test/main.cpp
    src/MSVC_RH_4_test.cpp
)

target_include_directories(MSVC_RH_4 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(MSVC_RH_4
    PRIVATE
        common
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)

# Add time testing executable
add_executable(MSVC_RH_4_timetest
    src/MSVC_RH_4.cpp
    src/MSVC_RH_4_timetest.cpp
)

target_include_directories(MSVC_RH_4_timetest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MSVC_RH_4_timetest
    PRIVATE
        common
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)