include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

add_executable(tide_info tide_info.cpp)
target_link_libraries(tide_info tide ${Boost_LIBRARIES})

