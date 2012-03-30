find_package(GTest)
if(NOT GTEST_FOUND)
    message(STATUS "GoogleTest was not found; tests will not be built.")
    return()
endif(NOT GTEST_FOUND)

include_directories(${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${GTEST_INCLUDE_DIRS})

set(srcs )

set(test_consts "${CMAKE_CURRENT_BINARY_DIR}/test_consts.h")
configure_file("test_consts.h.in" ${test_consts})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(all_tests ${srcs} ${test_consts})
target_link_libraries(all_tests tide ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES})
if(CMAKE_VERSION VERSION_LESS 2.8.4)
    add_test(AllTests all_tests)
else(CMAKE_VERSION VERSION_LESS 2.8.4)
    add_test(NAME AllTests COMMAND ${CMAKE_CURRENT_BINARY_DIR}/all_tests)
endif(CMAKE_VERSION VERSION_LESS 2.8.4)

