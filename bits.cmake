cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(tuxnbits LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FRAME "${PROJECT_NAME}++")
set(WORK "bits")
set(TARGET_DIR ${FRAME}/${WORK})

set(HEADERS
    ${TARGET_DIR}/export.h
    ${TARGET_DIR}/cadres.h
    ${TARGET_DIR}/cmdargs.h
    ${TARGET_DIR}/colors.h
    ${TARGET_DIR}/notify.h
    ${TARGET_DIR}/expect.h
    ${TARGET_DIR}/geometry.h
    ${TARGET_DIR}/glyphes.h
    ${TARGET_DIR}/databits/databits.h
    ${TARGET_DIR}/databits/field.h
    ${TARGET_DIR}/databits/table.h
    ${TARGET_DIR}/diagnostic.h
    ${TARGET_DIR}/diagnostic.inc
    ${TARGET_DIR}/object.h
    ${TARGET_DIR}/rem.h
    ${TARGET_DIR}/string.h
    ${TARGET_DIR}/appbits.h
    tuxnbits++/tui/twidgets/twidget.cc
    tuxnbits++/tui/twidgets/twidget.h
)


add_library(${PROJECT_NAME} STATIC

            ${HEADERS}
            ${TARGET_DIR}/colors.cc
            ${TARGET_DIR}/geometry.cc
            ${TARGET_DIR}/databits/databits.cc
            ${TARGET_DIR}/databits/field.cc
            ${TARGET_DIR}/databits/table.cc
            ${TARGET_DIR}/diagnostic.cc
            ${TARGET_DIR}/cadres.cc
            ${TARGET_DIR}/glyphes.cc
            ${TARGET_DIR}/object.cc
            ${TARGET_DIR}/rem.cc
            ${TARGET_DIR}/cmdargs.cc
            ${TARGET_DIR}/string.cc
            ${TARGET_DIR}/appbits.cc
            tuxnbits++/tui/twidgets/twidget.cc
            tuxnbits++/tui/twidgets/twidget.h

)
target_include_directories(${PROJECT_NAME} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include/${TARGET_DIR}>
)
message("build and include dir:" ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}++.${WORK})



target_compile_definitions("${PROJECT_NAME}" PUBLIC "${PROJECT_NAME}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${PROJECT_NAME}_export")

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()


target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} sqlite3) # and other system dependencies...





install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

INSTALL(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR})


add_custom_target("uninstall_${PROJECT_NAME}" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall_${PROJECT_NAME}"
    POST_BUILD
    COMMENT "Uninstall ${PROJECT_NAME} files under include and lib"
    COMMAND rm -vrf ${CMAKE_INSTALL_PREFIX}/include/${TARGET_DIR} ${CMAKE_INSTALL_PREFIX}/lib/lib${PROJECT_NAME}++* || echo Nothing in install_manifest.txt to be uninstalled!
)


#add_subdirectory(test.app)
