cmake_minimum_required(VERSION 2.8.3)

project(hikari)

# setup version numbers
set(HIKARI_VERSION_MAJOR 0)
set(HIKARI_VERSION_MINOR 2)
set(HIKARI_VERSION_PATCH 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(HIKARI_GOOD_COMPILER_FLAGS "-std=c++11 -Wall -O -g")
set(HIKARI_DUMB_COMPILER_FLAGS "-W3")

if(WIN32)
    # always link statically on Windows
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

#
# MSVC-specific settings
#
# Remove security warnings with Visual C++
#
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HIKARI_DUMB_COMPILER_FLAGS}")

    if(NOT CMAKE_CXX_FLAGS MATCHES "/EHsc")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") 
    endif()

    add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HIKARI_GOOD_COMPILER_FLAGS}")
endif(MSVC)

#
# Apple-specific stuff
#
if( APPLE )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++" )
  set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++" )
  set( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++" )
endif( APPLE )

add_subdirectory( engine )
add_subdirectory( test )