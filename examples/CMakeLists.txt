include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

set(write_eg_name "${PROJECT_NAME_LOWER}_eg_write")
add_executable(${write_eg_name} write.cpp)
target_link_libraries(${write_eg_name} jonen ${Boost_LIBRARIES})
install(TARGETS ${write_eg_name}
    DESTINATION ${BIN_INSTALL_DIR}
    COMPONENT tools)

set(read_eg_name "${PROJECT_NAME_LOWER}_eg_read")
add_executable(${read_eg_name} read.cpp)
target_link_libraries(${read_eg_name} jonen ${Boost_LIBRARIES})
install(TARGETS ${read_eg_name}
    DESTINATION ${BIN_INSTALL_DIR}
    COMPONENT tools)

