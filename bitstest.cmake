set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Dir: " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bitstest.app
    bitstest.app/main.cc
)


target_include_directories(bitstest.app PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

message("Build dir : build/")

target_link_directories(bitstest.app PUBLIC build)
add_dependencies(bitstest.app tuxnbits io)


# TABARNAK! j'joubli toujours de linker les librairies dans le bon ordre!!!!!
target_link_libraries(bitstest.app
    tuxnbits++.bits
    tuxnbits++.io
)
