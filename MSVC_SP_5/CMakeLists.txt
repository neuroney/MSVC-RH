add_executable(MSVC_SP_5
    src/MSVC_SP_5.cpp
    test/main.cpp
    src/MSVC_SP_5_test.cpp
)

target_include_directories(MSVC_SP_5 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(MSVC_SP_5
    PRIVATE
        common
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)

# Add time testing executable
add_executable(MSVC_SP_5_timetest
    src/MSVC_SP_5.cpp
    src/MSVC_SP_5_timetest.cpp
)

target_include_directories(MSVC_SP_5_timetest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MSVC_SP_5_timetest
    PRIVATE
        common
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)