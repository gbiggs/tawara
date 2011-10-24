set(hdrs tide.h
    exceptions.h
    null_stream.h
    win_dll.h
    tide_file.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

