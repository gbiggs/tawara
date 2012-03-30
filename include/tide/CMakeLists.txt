set(config_h ${CMAKE_CURRENT_BINARY_DIR}/tide_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tide_config.h.in
    ${config_h})

set(hdrs ${config_h}
    win_dll.h
    exceptions.h
    vint.h
    ids.h
    id_utils.h
    element.h
    element_base.h
    element_utils.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

