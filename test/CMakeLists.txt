find_package(GTest)
if(NOT GTEST_FOUND)
    message(STATUS "GoogleTest was not found; tests will not be built.")
    return()
endif(NOT GTEST_FOUND)

include_directories(${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${GTEST_INCLUDE_DIRS})

set(srcs test_utils.cpp
    test_vint.cpp
    test_ebml_int.cpp
    test_el_ids.cpp
    test_element.cpp
    test_int_element.cpp
    test_uint_element.cpp
    test_float_element.cpp
    test_date_element.cpp
    test_string_element.cpp
    test_binary_element.cpp
    test_void_element.cpp
    test_ebml_element.cpp
    test_tide.cpp
    test_seek_element.cpp
    test_metaseek.cpp
    test_segment_info.cpp
    test_tracks.cpp
    test_track_entry.cpp
    test_track_operation.cpp
    test_block_impl.cpp
    test_simple_block.cpp
    test_block_additions.cpp
    test_block_group.cpp
    test_cluster.cpp
    test_memory_cluster.cpp
    test_segment.cpp
    test_attachments.cpp)

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

