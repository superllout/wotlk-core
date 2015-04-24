project(EasyWoW)

cmake_minimum_required(VERSION 2.8.11)

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

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
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
GetCompilerVersion(compiler_version)
if (compiler_version STREQUAL "")
    message(FATAL_ERROR "Failed to determine C/C++ compiler")
endif()

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
        add_definitions(-D_CRT_SECURE_NO_WARNINGS -DHAS_CXX0X /EHa )

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
        #Extract Revision
        execute_process(WORKING_DIRECTORY ${ROOT_PATH} COMMAND sh git_version.sh )

        # Set global definitions here
        add_definitions(-DHAS_CXX0X)

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wno-deprecated")

        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")

        if (ENABLE_DEBUG_SUGGESTIONS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} 
            -Wsuggest-attribute=const 
            -Wsuggest-final-methods 
            -Wsuggest-override 
            -Wsuggest-attribute=format 
            -Wmissing-format-attribute 
            -Wshadow
            -Wno-unused-parameter -Winit-self -Wconversion")

            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} 
            -Wsuggest-attribute=const 
            -Wsuggest-final-methods 
            -Wsuggest-override 
            -Wsuggest-attribute=format 
            -Wmissing-format-attribute
            -Wshadow
            -Wno-unused-parameter -Winit-self -Wconversion")
        endif()

        if (SHOW_ADDITIONAL_WARNINGS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Wno-ignored-qualifiers -Wunsafe-loop-optimizations")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Wno-implicit -Wno-ignored-qualifiers -Wunsafe-loop-optimizations")
        endif()

        if (OPTIMIZED_BUILD)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
        endif()

        # Setting specific options for GNU/LLVM compilers
        if(CMAKE_COMPILER_IS_GNUCXX)
            if(compiler_version GREATER "4.8")   # colorful output was implemented in gcc 4.9
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto ")
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=auto ")
            endif()
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-nested-anon-types -Wshorten-64-to-32")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-nested-anon-types -Wshorten-64-to-32")
        endif()
    endif()

    if(APPLE)
        set( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DHAVE_DARWIN")
        set( EXTRA_LIBS ${EXTRA_LIBS} "-framework Carbon" )
        set( CMAKE_MACOSX_RPATH 1)
    elseif( ( CMAKE_SYSTEM_NAME  STREQUAL "FreeBSD" ) OR ( CMAKE_SYSTEM_NAME STREQUAL kFreeBSD ) )
        set( GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DUSE_KQUEUE")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set( EXTRA_LIBS ${EXTRA_LIBS} dl)
        set( GLOBAL_DEFINES "${GLOBAL_DEFINES} -DUSE_EPOLL")
    endif()
endif()

#Pass our configurable variables to the C++ preprocessor.
add_definitions( ${GLOBAL_DEFINES} -DCONFDIR=\"${ARCEMU_CONFIGSFILE_PATH}\" -DPREFIX=\"${ARCEMU_SCRIPTLIB_PATH}\")

set(ZLIB_LIBRARIES "" CACHE INTERNAL "Zlib libraries")
set(ZLIB_INCLUDE_DIRS "" CACHE INTERNAL "Zlib include dirs")
set(PCRE_LIBRARIES "" CACHE INTERNAL "PCRE libraries")
set(PCRE_INCLUDE_DIRS "" CACHE INTERNAL "PCRE include dirs")
set(BZIP2_LIBRARIES "" CACHE INTERNAL "BZIP2 libraries")
set(BZIP2_INCLUDE_DIR "" CACHE INTERNAL "BZIP2 include dirs")

#mark them as advanced so they don't show up on the gui.
mark_as_advanced(
    ZLIB_LIBRARIES
    ZLIB_INCLUDE_DIRS
    PCRE_LIBRARIES
    PCRE_INCLUDE_DIRS
    BZIP2_LIBRARIES
    BZIP2_INCLUDE_DIR)

#Find our needed libs
IF( NOT WIN32 )
	find_package(ZLIB)
	find_package(BZip2)
	find_package(PCRE REQUIRED)
	find_package(OpenSSL REQUIRED)
	find_package(Threads REQUIRED)
	find_package(MySQL REQUIRED)
ELSE()
	add_subdirectory(zlib)
	add_subdirectory(pcre)
	SET(MYSQL_LIBRARY "")
ENDIF()

set(GLOBAL_INCLUDE_DIRS 
${ROOT_PATH}/src/arcemu-shared
${ROOT_PATH}/src/arcemu-world
${ROOT_PATH}/src/arcemu-logonserver
${DEPENDENCY_INCLUDE_ROOT}
${DEPENDENCY_INCLUDE_ROOT}/g3dlite
${DEPENDENCY_INCLUDE_ROOT}/vmaplib
${DEPENDENCY_INCLUDE_ROOT}/pcre
${ZLIB_INCLUDE_DIRS}
${DEPENDENCY_INCLUDE_ROOT}/recast
${DEPENDENCY_INCLUDE_ROOT}/detour )

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
