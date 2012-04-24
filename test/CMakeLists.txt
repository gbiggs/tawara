find_package(GTest)
if(NOT GTEST_FOUND)
    message(STATUS "GoogleTest was not found; tests will not be built.")
    return()
endif(NOT GTEST_FOUND)

include_directories(${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${GTEST_INCLUDE_DIRS})


# This target simple tests if the templates can compile for simple
# instantiation, effectively providing an early warning system before hitting
# the full test suite with its more complex code (which leads to far more error
# messages to sort through). It does so by instantiating them, which will cause
# a compiler error if they can't compile. Tests of if the methods compile
# require using the methods, and so are left to the full test suite.
add_executable(compilable_test compilable_test.cpp)
target_link_libraries(compilable_test tide ${Boost_LIBRARIES})


set(srcs
    test_id_utils.cpp
    test_utilities.cpp
    test_vint.cpp
    test_ebml_integer.cpp
    test_int_element.cpp
    test_uint_element.cpp
    test_float_element.cpp
    test_date_element.cpp)

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

