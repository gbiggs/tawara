set(config_h ${CMAKE_CURRENT_BINARY_DIR}/tide_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tide_config.h.in
    ${config_h})

set(hdrs ${config_h}
    tide.h
    exceptions.h
    win_dll.h
    tide_impl.h
    vint.h
    ebml_int.h
    element.h
    prim_element.h
    int_element.h
    uint_element.h
    float_element.h
    string_element.h
    date_element.h
    binary_element.h
    ebml_element.h
    metaseek.h
    seek_element.h
    segment_info.h
    tracks.h
    track_entry.h
    track_operation.h
    segment.h
    block.h
    block_impl.h
    block_additions.h
    block_group.h
    cluster.h
    memory_cluster.h
    simple_block.h)

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

