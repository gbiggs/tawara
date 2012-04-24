set(config_h ${CMAKE_CURRENT_BINARY_DIR}/tide_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tide_config.h.in
    ${config_h})

set(hdrs ${config_h}
    win_dll.h
    exceptions.h
    vint.h
    ids.h
    id_utils.h
    ebml_integer.h
    element.h
    element_base.h
    element_utils.h
    primitive_element.h
    primitive_element_base.h
    integral_policies.h
    integral_element_impl.h
    integral_element.h
    integer_elements.h
    float_element.h
    float_element_impl.h
    date_element.h
    date_element_impl.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

