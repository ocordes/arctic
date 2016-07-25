cmake_minimum_required (VERSION 2.6)
project( arctic )

set(VERSION 1.3.0)

# get current date
execute_process(COMMAND "date" "+%Y-%m-%d" OUTPUT_VARIABLE BUILD)
string( REPLACE "\n" "" BUILD ${BUILD})
message(STATUS "Compilation date = ${BUILD}")


# Compile with C++11 support.  Disable __STRICT_ANSI__ to support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# add local cmake modules
LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

# define the target and parameter

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables
# `sources`

file(GLOB_RECURSE sources src/*.cc src/*.h)

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-Wall -Wextra -Wshadow)
ENDIF()

add_executable( arctic ${sources} )

# just for example add some compiler flags
#target_compile_options( arctic  PUBLIC  -g -O2 -std=gnu++11 -Wall )
target_compile_options( arctic  PUBLIC  -g -O3 -Wall )



# this lets me include files relative to the root src dir with a <> pair
target_include_directories( arctic PUBLIC src )


# add libraries
find_package(CCFITS REQUIRED)
include_directories(${CCFITS_INCLUDE_DIR})
target_link_libraries( arctic ${CCFITS_LIBRARIES})


# os configuration file
INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
#CHECK_INCLUDE_FILES (utime.h HAVE_UTIME_H)
CHECK_INCLUDE_FILES (sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES (sys/resource.h HAVE_SYS_RESOURCE_H)
configure_file ("${PROJECT_SOURCE_DIR}/src/config_cmake.h.in"
                "${PROJECT_BINARY_DIR}/config.h" )
# Now make sure that you the the build directory on your "Include" path when compiling
include_directories(${PROJECT_BINARY_DIR})


# install part
install(TARGETS arctic DESTINATION bin)

# CPack part

# CPack version numbers for release tarball name.
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 3)
set(CPACK_PACKAGE_VERSION_PATCH 0)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Algorithm foR Charge Transfer Inefficency Correcton - arctic")
set(CPACK_PACKAGE_VENDOR "Oliver Cordes/Ole Marggraf")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_GENERATOR TGZ)
set(
CPACK_SOURCE_PACKAGE_FILE_NAME "arctic-${VERSION}"
CACHE INTERNAL "tarball basename"
)
set(CPACK_SOURCE_GENERATOR TGZ)
# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
set(CPACK_SOURCE_IGNORE_FILES
"~$"
"\\\\.cvsignore$"
"^${PROJECT_SOURCE_DIR}.*/CVS/"
"^${PROJECT_SOURCE_DIR}/debian/"
"^${PROJECT_SOURCE_DIR}/build/"
"^${PROJECT_SOURCE_DIR}/demo/"
"^${PROJECT_SOURCE_DIR}/test_suite/data/*.fitsq"
"^${PROJECT_SOURCE_DIR}/tests/"
)
#message("CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
