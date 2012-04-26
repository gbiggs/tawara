set(srcs
    vint.cpp
    id_utils.cpp
    ebml_integer.cpp
    element.cpp
    element_utils.cpp
    float_element.cpp
    float_element_impl.cpp
    date_element.cpp
    date_element_impl.cpp
    binary_element.cpp
    binary_element_impl.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

add_library(${PROJECT_NAME_LOWER} ${LIB_TYPE} ${srcs})
target_link_libraries(${PROJECT_NAME_LOWER} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME_LOWER}
    EXPORT ${PROJECT_NAME_LOWER}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT library
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT library
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT library)
install(EXPORT ${PROJECT_NAME_LOWER}
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    FILE ${PROJECT_NAME_LOWER}Depends.cmake)

