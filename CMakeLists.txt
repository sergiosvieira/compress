cmake_minimum_required(VERSION 3.0)
project(zipper)
set(ENV{PUBLIC_HEADERS})

# FUNCTIONS
# =========
function(_get_headers)
    file(GLOB HEADERS "*.h")
    list(LENGTH HEADERS HEADERS_LENGTH)
    if(${HEADERS_LENGTH} GREATER 0)
        set(ENV{PUBLIC_HEADERS} "$ENV{PUBLIC_HEADERS};${HEADERS}")
    endif(${HEADERS_LENGTH} GREATER 0)
endfunction(_get_headers)

MACRO(_add_subdirectories curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  FOREACH(subdir ${dirlist})
    ADD_SUBDIRECTORY(${subdir})
  ENDFOREACH()
ENDMACRO()

# GETTING SYSTEM ARCHITECTURE
# ===========================
if(WINDOWS)
    set(platform "win")
elseif(APPLE)
    set(platform "mac")
elseif(UNIX)
    # get linux distrog
    if(EXISTS "/etc/issue")
        file(READ "/etc/issue" contents)
        string(TOLOWER ${contents} contents)
        if(${contents} MATCHES "ubuntu")
            set(platform "ubuntu")
        elseif(${contents} MATCHES "debian")
            set(platform "debian")
        elseif(${contents} MATCHES "RedHat")
            set(platform "redhat")
        else()
            message(FATAL_ERROR "This linux distro (${contents}) is not supported!\nSupported distros are: Ubuntu, Debian and Red Hat.")
        endif()
   else(EXISTS "/etc/issue")
        message(FATAL_ERROR "Platform not supported! The platforms supporteds are: Windows 7 and below, Ubuntu, Red Hat, Debian and Mac")
   endif(EXISTS "/etc/issue")
endif(WINDOWS)

# SETTTING BINARY OUTPUT DIRECTORY
# ===============================
if(${CMAKE_BUILD_TYPE} MATCHES "RELEASE" )
    set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/release/${platform} )
    set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/release/${platform} )
else(${CMAKE_BUILD_TYPE} MATCHES "DEBUG" )
    set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/debug/${platform} )
    set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/debug/${platform} )
endif()

# COMPILER DIRECTIVES
# ===================
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Cland")
    set(warnings "-Wall -Wextra -std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /EHsc")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

# GETTING DEPENDENCIES
# ====================
find_package(LibArchive REQUIRED)

# INCLUDE LIBRARIES
# =================
include_directories(${LibArchive_INCLUDE_DIRS})
include_directories(include)

# MAIN STRUCTURE
# ==============
add_subdirectory(include)
add_subdirectory(src)
