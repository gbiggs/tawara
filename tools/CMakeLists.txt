include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

add_executable(jonen_info jonen_info.cpp)
target_link_libraries(jonen_info jonen ${Boost_LIBRARIES})
install(TARGETS jonen_info
    DESTINATION ${BIN_INSTALL_DIR}
    COMPONENT tools)

