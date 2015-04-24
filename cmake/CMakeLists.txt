project(EasyWoW C CXX)

cmake_minimum_required(VERSION 2.8)
#Do system checking.

MACRO( IS_ARCH_64BIT )
    if( NOT WIN32 )
        
        if( CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[aA][mM][dD]64" )
            set( IS_64BIT TRUE )
        else()
            set( IS_64BIT FALSE )
        endif()    
    else()
        if( CMAKE_GENERATOR MATCHES Win64* )
            set( IS_64BIT TRUE )
        else()
            set( IS_64BIT FALSE )
        endif()
        
    endif()
    
ENDMACRO( IS_ARCH_64BIT )

set(GLOBAL_DEFINES "${GLOBAL_DEFINES} -DHAVE_CONFIG_H")

if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Release Debug)
endif()

IS_ARCH_64BIT()

#check platform version.
if( IS_64BIT )
    set( GLOBAL_DEFINES ${GLOBAL_DEFINES} -"DX64")
endif()

set( ROOT_PATH  ${CMAKE_SOURCE_DIR}/..)

#We have our own custom modules that we use. This tells cmakes where to find them.
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ROOT_PATH}/cmake_modules )
set( DEPENDENCY_ROOT ${ROOT_PATH}/dep )
set( DEPENDENCY_SOURCES ${DEPENDENCY_ROOT}/src)
set( DEPENDENCY_LIBS ${DEPENDENCY_ROOT}/lib)
set( DEPENDENCY_INCLUDE_ROOT ${DEPENDENCY_ROOT}/include)
set( DEPENDENCY_DLLS64 ${DEPENDENCY_ROOT}/dll64)

set(ARCEMU_SCRIPTLIB_PATH "script_libs" CACHE PATH "The directory which contains the 'lib' directory that has the script libraries." )

#Options
OPTION(BUILD_ARCEMUSCRIPTS "set to false to NOT build script libraries." ON)
OPTION(BUILD_TOOLS "Build Arcemu tools" ON)
OPTION(OPTIMIZED_BUILD "set to true for optimized compilation - will take more time" OFF)
OPTION(SHOW_ADDITIONAL_WARNINGS "set to true to get all compiler warnings in debug build." FALSE)

# platforms specific options
if (WIN32)
    set(VISUALSTUDIO_COMPILERHEAPLIMIT 400 CACHE STRING "Visual Studio compiler heap limit. Ignore on darwin and unix platforms.")
elseif(UNIX)
    OPTION(ENABLE_CODE_SUGGESTIONS "set to true to get compiler suggestions for code" OFF)
endif()

include(CompilerVersion)
if( WIN32 )
    set(ARCEMU_CONFIGSFILE_PATH config CACHE PATH "Path where the arcemu config files are.")

    # setting default install path
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "C:/EasyWoW" CACHE PATH "Install path prefix" FORCE)
    endif()

    # Install libmysql.dll required for our core to run.
    if(IS_64BIT)
        set(DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll64)
    else()
        set(DEPENDENCY_DLLS ${DEPENDENCY_ROOT}/dll)
    endif()
    set( INSTALLED_DEPENDENCIES ${DEPENDENCY_DLLS}/libmysql.dll ${DEPENDENCY_DLLS}/libeay32.dll)
    

    # settings for visual studio
    if( CMAKE_GENERATOR MATCHES Visual* )
        #Extract revision
        execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND git_version.bat )
        add_definitions(-D_CRT_SECURE_NO_WARNINGS /EHa )

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /wd4669 /wd4127 /wd4996 /we4189")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4669 /wd4127 /wd4996 /we4189")

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /bigobj")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
        if(IS_64BIT)
            set( EXTRA_LIBS libeay32_win64.lib )
            if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                set(EXTRA_LIBS ${EXTRA_LIBS} libmysql_debug_x64.lib )
            else()
                set( EXTRA_LIBS ${EXTRA_LIBS} libmysql_release_x64.lib )
            endif()
        else()
            set( EXTRA_LIBS libeay32_win32.lib )
            if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                set(EXTRA_LIBS ${EXTRA_LIBS} libmysql_debug_win32.lib )
            else()
                set( EXTRA_LIBS ${EXTRA_LIBS} libmysql_release_win32.lib )
            endif()
        endif()
    
        #Check support for unordered_map/set
        GetCompilerVersion( msvc_version)
        if(DEFINED ${msvc_version} AND ${msvc_version} GREATER "1599" )
            add_definitions(-DHAS_CXX0X)
        endif()
        
        #This fixes PCH issues 'Inconsistent values for /Zm'
        if( ${CMAKE_CXX_FLAGS} MATCHES "(/Zm)([0-9]+)" )
            STRING( REGEX REPLACE "(/Zm)([0-9]+)" "\\1${VISUALSTUDIO_COMPILERHEAPLIMIT}" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
        else()
            set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm${VISUALSTUDIO_COMPILERHEAPLIMIT}"  )
        endif()
            
        if( ${CMAKE_C_FLAGS} MATCHES "(/Zm)([0-9]+)" )
            STRING( REGEX REPLACE "(/Zm)([0-9]+)" "\\1${VISUALSTUDIO_COMPILERHEAPLIMIT}" CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )
        else()
            set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zm${VISUALSTUDIO_COMPILERHEAPLIMIT}"  )
        endif()

        if (NOT SHOW_ADDITIONAL_WARNINGS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3")
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Wall")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Wall")
        endif()

        if (OPTIMIZED_BUILD)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
        endif()
    endif()
    #Needed for socket stuff and crash handler
    set(EXTRA_LIBS ${EXTRA_LIBS} ws2_32.lib dbghelp.lib )
    set( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} 
        ${DEPENDENCY_INCLUDE_ROOT}/openssl
        ${DEPENDENCY_INCLUDE_ROOT}/mysql 
    )
# UNIX platforms
elseif(UNIX)
    set(ARCEMU_CONFIGSFILE_PATH etc CACHE PATH "Path where the arcemu config files are.")
    set( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} )

    if( CMAKE_GENERATOR MATCHES Unix* )
        add_definitions(-Wno-deprecated)
        #Extract Revision
        execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND sh git_version.sh )
        
        #Check support for unordered_map/set
        # placeholder for future additions
        # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuseless-cast")

        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wno-unused-parameter -Winit-self -Wconversion")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wno-unused-parameter -Winit-self -Wconversion")

        if (ENABLE_DEBUG_SUGGESTIONS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} 
            -Wsuggest-attribute=const 
            -Wsuggest-final-methods 
            -Wsuggest-override 
            -Wsuggest-attribute=format 
            -Wmissing-format-attribute 
            -Wshadow")

            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} 
            -Wsuggest-attribute=const 
            -Wsuggest-final-methods 
            -Wsuggest-override 
            -Wsuggest-attribute=format 
            -Wmissing-format-attribute
            -Wshadow")
        endif()

        if (SHOW_ADDITIONAL_WARNINGS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Wno-implicit -Wno-ignored-qualifiers -Wunsafe-loop-optimizations")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Wno-implicit -Wno-ignored-qualifiers -Wunsafe-loop-optimizations")
        endif()

        if (OPTIMIZED_BUILD)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
        endif()
        GetCompilerVersion(compiler_version)
        if( DEFINED compiler_version)
            add_definitions(-DHAS_CXX0X)
            # Enabling c++0x
            if(CMAKE_C_COMPILER MATCHES "gcc" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
                if (${compiler_version} GREATER "4.2" AND ${compiler_version} LESS "4.7") 
                    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
                    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  
                # Enabling c++11
                elseif(${compiler_version} GREATER "4.6")
                    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
                    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
                    if(compiler_version GREATER "4.8")   # colorful output was implemented in gcc 4.9
                        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto ")
                        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=auto ")
                    endif()
                endif()
            elseif(CMAKE_C_COMPILER MATCHES "clang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
                add_definitions(-DHAS_CXX0X)
                if(${compiler_version} GREATER "3.1" AND ${compiler_version} LESS "3.5")
                    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
                    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
                elseif(${compiler_version} GREATER "3.4")
                    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
                    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
                endif()
                set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-nested-anon-types -Wshorten-64-to-32")
                set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-nested-anon-types -Wshorten-64-to-32")
            endif()
        endif()
    endif()

    if(APPLE)
        set( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DHAVE_DARWIN")
        set( EXTRA_LIBS ${EXTRA_LIBS} "-framework Carbon" )
        set( CMAKE_MACOSX_RPATH 1)
        if( NOT IS_64BIT )
            set( EMPTY "" )
            STRING( REGEX REPLACE ".*-m64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}" )
            STRING( REGEX REPLACE ".*-m 64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}" )
            STRING( REGEX REPLACE ".*-m64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
            STRING( REGEX REPLACE ".*-m 64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
            STRING( REGEX REPLACE ".*-m64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
            STRING( REGEX REPLACE ".*-m 64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
            STRING( REGEX REPLACE ".*-m64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}" )
            STRING( REGEX REPLACE ".*-m 64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}" )        
            STRING( REGEX REPLACE ".*-m64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}" )
            STRING( REGEX REPLACE ".*-m 64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}" )
            ADD_DEFINITIONS( -m32 )
            set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32" )
            set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -undefined dynamic_lookup" )
            set( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32" )
        endif()
    elseif( ( CMAKE_SYSTEM_NAME  STREQUAL "FreeBSD" ) OR ( CMAKE_SYSTEM_NAME STREQUAL kFreeBSD ) )
        set( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DUSE_KQUEUE")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set( EXTRA_LIBS ${EXTRA_LIBS} dl)
        set( GLOBAL_DEFINES "${GLOBAL_DEFINES} -DUSE_EPOLL")
    endif()
endif()

#Pass our configurable variables to the C++ preprocessor.
add_definitions( ${GLOBAL_DEFINES} -DCONFDIR=\"${ARCEMU_CONFIGSFILE_PATH}\" -DPREFIX=\"${ARCEMU_SCRIPTLIB_PATH}\")

set(ZLIB_LIBRARIES "" CACHE INTERNAL "Zlib libraries.")
set(PCRE_LIBRARIES "" CACHE INTERNAL "PCRE libraries.")
set(PCRE_INCLUDE_DIRS "" CACHE INTERNAL "PCRE include dirs.")
set(BZIP2_INCLUDE_DIRS "" CACHE INTERNAL "BZIP2 include dirs.")
#mark them as advanced so they don't show up on the gui.
mark_as_advanced(ZLIB_LIBRARIES PCRE_LIBRARIES PCRE_INCLUDE_DIRS BZIP2_INCLUDE_DIR)

set(GLOBAL_INCLUDE_DIRS 
${ROOT_PATH}/src/arcemu-shared
${ROOT_PATH}/src/arcemu-world
${ROOT_PATH}/src/arcemu-logonserver
${DEPENDENCY_INCLUDE_ROOT}
${DEPENDENCY_INCLUDE_ROOT}/g3dlite
${DEPENDENCY_INCLUDE_ROOT}/vmaplib
${DEPENDENCY_INCLUDE_ROOT}/pcre
${DEPENDENCY_INCLUDE_ROOT}/zlib
${DEPENDENCY_INCLUDE_ROOT}/recast
${DEPENDENCY_INCLUDE_ROOT}/detour )

#Find our needed libs
IF( NOT WIN32 )
	find_package(ZLIB REQUIRED)
	find_package(BZip2 REQUIRED)
	find_package(PCRE REQUIRED)
	find_package(OpenSSL REQUIRED)
	find_package(Threads REQUIRED)
	find_package(MySQL REQUIRED)
ELSE()
	add_subdirectory(zlib)
	add_subdirectory(pcre)
	SET(MYSQL_LIBRARY "")
ENDIF()

add_subdirectory(g3dlite)
add_subdirectory(vmaplib)
add_subdirectory(recast)
add_subdirectory(detour)
add_subdirectory(shared)
add_subdirectory(logon)
add_subdirectory(world)
add_subdirectory(spellhandlers)
add_subdirectory(battlegrounds)

if(NOT WIN32)
    add_subdirectory(crashreport)
endif()
add_subdirectory(scripts)

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# install dependencies
INSTALL(FILES ${INSTALLED_DEPENDENCIES} DESTINATION .)

# install configs
install(FILES "${ROOT_PATH}/configs/world.conf.install" DESTINATION ${ARCEMU_CONFIGSFILE_PATH})
install(FILES "${ROOT_PATH}/configs/optional.conf.install" DESTINATION ${ARCEMU_CONFIGSFILE_PATH})
install(FILES "${ROOT_PATH}/configs/realms.conf.install" DESTINATION ${ARCEMU_CONFIGSFILE_PATH})
install(FILES "${ROOT_PATH}/configs/logon.conf.install" DESTINATION ${ARCEMU_CONFIGSFILE_PATH})
