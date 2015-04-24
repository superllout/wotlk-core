PROJECT(libmpq_new)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
common.c
explode.c
extract.c
huffman.c
mpq.c
mpq_libmpq.cpp
wave.c
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

IF( NOT WIN32 AND NOT IS_64BIT )
	add_definitions( -D_FILE_OFFSET_BITS=64 )
ENDIF( NOT WIN32 AND NOT IS_64BIT )

include_directories( ${DEPENDENCY_INCLUDE_ROOT}/${PROJECT_NAME} ${DEPENDENCY_INCLUDE_ROOT}/zlib ${BZIP2_INCLUDE_DIR} ${DEPENDENCY_INCLUDE_ROOT}/loadlib )
add_library(${PROJECT_NAME} STATIC ${SRCS} )
target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES})

unset(sources)
unset(SRCS)
unset(prefix)

