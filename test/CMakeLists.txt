find_package(GTest)
if(NOT GTEST_FOUND)
    message(STATUS "GoogleTest was not found; tests will not be built.")
    return()
endif(NOT GTEST_FOUND)

include_directories(${PROJECT_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

set(srcs file_open.cpp)

add_executable(all_tests ${srcs})
target_link_libraries(all_tests tide ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES})
if(CMAKE_VERSION VERSION_LESS 2.8.4)
    add_test(AllTests all_tests)
else(CMAKE_VERSION VERSION_LESS 2.8.4)
    add_test(NAME AllTests COMMAND ${CMAKE_CURRENT_BINARY_DIR}/all_tests)
endif(CMAKE_VERSION VERSION_LESS 2.8.4)

