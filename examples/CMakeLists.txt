include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

set(write_eg_name "${PROJECT_NAME_LOWER}_eg_write")
add_executable(${write_eg_name} write.cpp)
target_link_libraries(${write_eg_name} tide ${Boost_LIBRARIES})

set(read_eg_name "${PROJECT_NAME_LOWER}_eg_read")
add_executable(${read_eg_name} read.cpp)
target_link_libraries(${read_eg_name} tide ${Boost_LIBRARIES})

