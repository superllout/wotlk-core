PROJECT(bzip2)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
blocksort.c
bzlib.c
compress.c
crctable.c
decompress.c
huffman.c
randtable.c
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} )

set(BZIP2_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "bzip2 Libraries")
set(BZIP2_INCLUDE_DIR ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} CACHE INTERNAL "bzip2 include directories")

unset(sources)
unset(SRCS)
unset(prefix)

