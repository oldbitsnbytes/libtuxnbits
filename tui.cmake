
set(FRAME "tuxnbits++")
set(WORK   "tui")



project(${WORK} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_DIR ${FRAME}/${WORK})
set(TARGETNAME ${WORK})

message(STATUS "TARGET_DIR: ${TARGET_DIR}")

set(HEADERS
    ${TARGET_DIR}/export.h
    ${TARGET_DIR}/tuins.h
    ${TARGET_DIR}/pencil.h
    ${TARGET_DIR}/event.h
    ${TARGET_DIR}/event.inline
    ${TARGET_DIR}/twidgets/twidget.h
)

#set(OUT ${FRAME}_${WORK})
add_library(${TARGETNAME} STATIC

    ${HEADERS}
    ${TARGET_DIR}/tuins.cc
    ${TARGET_DIR}/pencil.cc
    ${TARGET_DIR}/event.cc
    ${TARGET_DIR}/twidgets/twidget.cc
)



target_include_directories(${TARGETNAME} PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           $<INSTALL_INTERFACE:include/${TARGET_DIR}>
)


set_target_properties(${TARGETNAME} PROPERTIES OUTPUT_NAME "${FRAME}.${WORK}")

target_compile_definitions(${TARGETNAME} PUBLIC "${TARGETNAME}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header(${TARGETNAME} EXPORT_FILE_NAME "${TARGETNAME}_export")

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()

# static .a should have been put into the current project archive file (libluss.io.a). So no need to link again with luss.a in the other project.
target_link_libraries(${TARGETNAME} ${CMAKE_DL_LIBS} tuxnbits++.bits tuxnbits++.io sqlite3)





install(TARGETS ${TARGETNAME}
        EXPORT  ${TARGETNAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

INSTALL(FILES ${HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR}")


add_custom_target("uninstall_${TARGETNAME}" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall_${TARGETNAME}"
    POST_BUILD
    COMMENT "Uninstall ${TARGETNAME} files under include and lib"
    COMMAND rm -vrf ${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR} ${CMAKE_INSTALL_PREFIX}/lib/lib${TARGETNAME}* || echo Nothing in install_manifest.txt to be uninstalled!
)


#add_subdirectory(test.app)
