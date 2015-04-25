#child CMakeLists use this variable to access the source file directories. 
#e.g ${SCRIPTS_PATH}/LuaEngine to access the source files for LuaEngine.

if( NOT BUILD_ARCEMUSCRIPTS )
  message( WARNING, "BUILD_ARCEMUSCRIPTS option is set to false. Script libraries will not be built." )
  return()
endif(NOT BUILD_ARCEMUSCRIPTS)
message("Configuring scripts :")
SET( SCRIPTS_PATH ${ROOT_PATH}/src/scripts )
#common include directories that script libraries might include.
SET( include_dirs ${GLOBAL_INCLUDE_DIRS} )
SET( dependancies shared world )
SET( linkages world)

#All script libraries have SCRIPTLIB defined.
add_definitions(-DSCRIPTLIB -DSKIP_ALLOCATOR_SHARING)
SET( BUILD_EVENTSCRIPTS TRUE CACHE BOOL "Build arcEventScripts.")
SET( BUILD_INSTANCESCRIPTS TRUE CACHE BOOL "Build arcInstanceScripts.")
SET( BUILD_EXTRASCRIPTS TRUE CACHE BOOL "Build arcExtraScripts.")
SET( BUILD_GOSSIPSCRIPTS TRUE CACHE BOOL "Build arcGossipScripts.")
SET( BUILD_QUESTSCRIPTS TRUE CACHE BOOL "Build arcQuestScripts.")
SET( BUILD_MISCSCRIPTS TRUE CACHE BOOL "Build arcMiscScripts." )
SET( BUILD_SERVERSTATUSPLUGIN FALSE CACHE BOOL "Build ServerStatusPlugin.")
SET( BUILD_LUAENGINE FALSE CACHE BOOL "Build Arcemu Lua Engine ( ALE ).")
# SET( BUILD_LUABRIDGE FALSE CACHE BOOL "Build luabridge.")


add_subdirectory( eventscripts)
add_subdirectory( instancescripts )
add_subdirectory( extrascripts )
add_subdirectory( gossipscripts )
add_subdirectory( questscripts )
add_subdirectory( miscscripts )
add_subdirectory( serverstatusplugin )

#Used by luabridge and luaengine modules.
if (UNIX)
    if ((NOT USE_EXTERNAL_LUA AND BUILD_LUAENGINE) OR (USE_EXTERNAL_LUA AND NOT LUA_FOUND))
        if(NOT LUA_FOUND)
            message(WARNING "Lua was not found, using core preadded dependency")
        endif()
        add_subdirectory( lualib )
    endif()
elseif(WIN32)
    add_subdirectory( lualib )
endif()

# add_subdirectory( luabridge ) 
add_subdirectory( luaengine )

unset(SCRIPTS_PATH)
unset(include_dirs)
