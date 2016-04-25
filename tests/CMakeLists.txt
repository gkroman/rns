# -*- indent-tabs-mode: nil; -*-
# vim: ts=4:sts=4:sw=4:et

set(LIBS rns rt zmq pthread)

file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(BINARIES)

foreach(SRC ${SOURCES})
    string(REPLACE ".cpp" ".bin" BIN ${SRC})
    add_executable(${BIN} ${SRC})
    target_link_libraries(${BIN} ${LIBS})
    set(BINARIES ${BINARIES} ${BIN})
endforeach(SRC ${SOURCES})

install(TARGETS ${BINARIES} DESTINATION bin)
