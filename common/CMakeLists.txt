add_library(common
    src/helper.cpp
)

add_executable(common_tests
    test/MultiPolyTests.cpp
)

target_include_directories(common_tests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common
    PRIVATE
        ${NTL_LIBRARIES}
        ${GMP_LIBRARIES}
        m
)

