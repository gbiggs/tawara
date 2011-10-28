set(srcs tide_file.cpp
    vint.cpp
    ebml_int.cpp
    element.cpp
    uint_element.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME_LOWER} ${LIB_TYPE} ${srcs})

install(TARGETS ${PROJECT_NAME_LOWER}
    EXPORT ${PROJECT_NAME_LOWER}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT library
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT library
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT library)
install(EXPORT ${PROJECT_NAME_LOWER}
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    FILE ${PROJECT_NAME_LOWER}Depends.cmake)

