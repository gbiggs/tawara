include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

add_executable(tawara_info tawara_info.cpp)
target_link_libraries(tawara_info tawara ${Boost_LIBRARIES})
install(TARGETS tawara_info
    DESTINATION ${BIN_INSTALL_DIR}
    COMPONENT tools)

