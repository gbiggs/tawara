set(hdrs tide.h
    exceptions.h
    win_dll.h
    tide_file.h
    vint.h
    ebml_int.h
    element.h
    prim_element.h
    ebml_element.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

