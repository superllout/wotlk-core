SET(ARCEMU_TOOLS_PATH "tools" CACHE PATH "The directory where you want the tools installed." )
SET(BUILD_TOOLS_MAPS TRUE CACHE BOOL "Build DBC and Map extractors." )
SET(BUILD_TOOLS_VMAPS TRUE CACHE BOOL "Build VMAP extractor tools." )
SET(BUILD_TOOLS_CREATUREDATA TRUE CACHE BOOL "Build creature data extractor tools" )

if (WIN32)
 add_subdirectory( bzip2 )
endif()

add_subdirectory( libmpq_new )
add_subdirectory( dbcfile )
add_subdirectory( loadlib )

IF( BUILD_TOOLS_MAPS )
	add_subdirectory( map_extractor )
ENDIF(BUILD_TOOLS_MAPS)

IF( BUILD_TOOLS_CREATUREDATA )
	add_subdirectory( creature_data )
ENDIF( BUILD_TOOLS_CREATUREDATA )

IF( BUILD_TOOLS_VMAPS )
	add_subdirectory( vmap_tools )
ENDIF( BUILD_TOOLS_VMAPS )

