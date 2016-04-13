# -*- indent-tabs-mode: nil; -*-
# vim: se ts=4:sts=4:sw=4:et

file(GLOB_RECURSE SOURCES *.cpp)

set(VERSION "1.0.0")

add_library(rns STATIC ${SOURCES})
set_target_properties(rns PROPERTIES
	VERSION ${VERSION}
	FRAMEWORK false
)

install(TARGETS rns DESTINATION lib)
