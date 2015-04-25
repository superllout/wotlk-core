
set(REQUIRED_MIN_GCC_VERSION "4.7.2")
set(REQUIRED_MIN_CLANG_VERSION "3.4")
set(REQUIRED_MIN_MSVC_VERSION "1800")
set(REQUIRED_MIN_MSVC_NAME "Visual studio 2013")

macro( GetCompilerVersion out_version)

	set(COMPILER_FOUND FALSE)   # this variable idicates about founded compiler, if value is false it will produces cmake error
    if(UNIX)
	    IF(CMAKE_COMPILER_IS_GNUCXX)
	        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS REQUIRED_MIN_GCC_VERSION)
                message(FATAL_ERROR "GCC: EasyWoW requires version ${REQUIRED_MIN_GCC_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
            endif()

		    EXECUTE_PROCESS( COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE tmp_version )
		    SET(${out_version} ${tmp_version} )
		    SET(COMPILER_FOUND TRUE)
	    ENDIF()

        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS REQUIRED_MIN_CLANG_VERSION)
                message(FATAL_ERROR "Clang: EasyWoW requires version ${REQUIRED_MIN_CLANG_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
            endif()

		    EXECUTE_PROCESS( COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE tmp_version )
		    SET(${out_version} ${tmp_version} )
		    SET(COMPILER_FOUND TRUE)
        endif()
    elseif(WIN32)
	    IF( MSVC )
	        if (MSVC_VERSION VERSION_LESS REQUIRED_MIN_MSVC_VERSION)
              message(FATAL_ERROR "Visual Studio: EasyWoW required version is ${REQUIRED_MIN_MSVC_NAME} but found ${CMAKE_CXX_COMPILER_VERSION}")
            endif()
		    SET(${out_version} ${MSVC_VERSION} )
		    set(COMPILER_FOUND TRUE)
		else()
              message(FATAL_ERROR "EasyWoW supports Windows platform builds only on Visual Studio 2013 or later versions")		    
	    endif()
    endif()

    if (NOT COMPILER_FOUND)
	    message(FATAL_ERROR "Required C/C++ compiler was not found")
    endif()

endmacro( GetCompilerVersion)
