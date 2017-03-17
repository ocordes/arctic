# Unit test dedfinitions

# paths where to locate scripts and executables
set(binary_paths)
foreach(modp ${CMAKE_MODULE_PATH})
  if(EXISTS ${modp}/scripts)
    set(binary_paths ${binary_paths} ${modp}/scripts)
endif()
endforeach()

list(REMOVE_DUPLICATES binary_paths)


find_program(Boost_testmain_cmd createBoostTestMain.py HINTS ${binary_paths})

# stolen from EuclidEnvironment and modified for  arctic

#-------------------------------------------------------------------------------
# get_required_include_dirs(<output> <libraries>)
#
# Get the include directories required by the linker libraries specified
# and prepend them to the output variable.
#-------------------------------------------------------------------------------
function(get_required_include_dirs output)
  set(collected)
  foreach(lib ${ARGN})
    set(req)
    if(TARGET ${lib})
      list(APPEND collected ${lib})
      get_property(req TARGET ${lib} PROPERTY REQUIRED_INCLUDE_DIRS)
      if(req)
        list(APPEND collected ${req})
      endif()
    endif()
  endforeach()
  if(collected)
    set(collected ${collected} ${${output}})
    list(REMOVE_DUPLICATES collected)
    set(${output} ${collected} PARENT_SCOPE)
  endif()
endfunction()

#-------------------------------------------------------------------------------
# get_required_library_dirs(<output> <libraries>)
#
# Get the library directories required by the linker libraries specified
# and prepend them to the output variable.
#-------------------------------------------------------------------------------
function(get_required_library_dirs output)
  set(collected)
  foreach(lib ${ARGN})
    set(req)
    # Note: adding a directory to the library path make sense only to find
    # shared libraries (and not static ones).
    if(EXISTS ${lib} AND lib MATCHES "${CMAKE_SHARED_LIBRARY_PREFIX}[^/]*${CMAKE_SHARED_LIBRARY_SUFFIX}\$")
      get_filename_component(req ${lib} PATH)
      if(req)
        list(APPEND collected ${req})
      endif()
      # FIXME: we should handle the inherited targets
      # (but it's not mandatory because they where already handled)
    #else()
    #  message(STATUS "Ignoring ${lib}")
    endif()
  endforeach()
  if(collected)
  set(${output} ${collected} ${${output}} PARENT_SCOPE)
endif()
endfunction()

#-------------------------------------------------------------------------------
# expand_sources(<variable> source_pattern1 source_pattern2 ...)
#
# Expand glob patterns for input files to a list of files, first searching in
# ``src`` then in the current directory.
#-------------------------------------------------------------------------------
macro(expand_sources VAR)
  set(${VAR})
  foreach(fp ${ARGN})
    file(GLOB files src/${fp})
    if(files)
      set(${VAR} ${${VAR}} ${files})
    else()
      file(GLOB files ${fp})
      if(files)
        set(${VAR} ${${VAR}} ${files})
      else()
        set(${VAR} ${${VAR}} ${fp})
      endif()
    endif()
  endforeach()
endmacro()

#-------------------------------------------------------------------------------
# common_add_build(sources...
#                 LINK_LIBRARIES library1 package2 ...
#                 INCLUDE_DIRS dir1 package2 ...)
#
# Internal. Helper macro to factor out the common code to configure a buildable
# target (library, module, dictionary...)
#-------------------------------------------------------------------------------
macro(common_add_build)
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS" ${ARGN})
  # obsolete option
  if(ARG_LIBRARIES)
    message(WARNING "Deprecated option 'LIBRARIES', use 'LINK_LIBRARIES' instead")
    set(ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${ARG_LIBRARIES})
  endif()

  #elements_resolve_link_libraries(ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES})

  # find the sources
  expand_sources(srcs ${ARG_UNPARSED_ARGUMENTS})

  #message(STATUS "elements_common_add_build ${ARG_LINK_LIBRARIES}")
  # get the inherited include directories
  get_required_include_dirs(ARG_INCLUDE_DIRS ${ARG_LINK_LIBRARIES})
  message(STATUS "elements_common_add_build ${ARG_LINK_LIBRARIES}")

  #message(STATUS "elements_common_add_build ${ARG_INCLUDE_DIRS}")
  # add the package includes to the current list
  #include_package_directories(${ARG_INCLUDE_DIRS})
  include_directories( ${ARG_INCLUDE_DIRS} )

  #message(STATUS "elements_common_add_build ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES}")
  # get the library dirs required to get the libraries we use
  get_required_library_dirs(lib_path ${ARG_LINK_LIBRARIES})
  set_property(GLOBAL APPEND PROPERTY LIBRARY_PATH ${lib_path})

endmacro()

#-----------------------------------------------------------------------------
# add_unit_test(<name>
#                     source1 source2 ...
#                     LINK_LIBRARIES library1 library2 ...
#                     INCLUDE_DIRS dir1 package2 ...
#                     [WORKING_DIRECTORY dir]
#                     [ENVIRONMENT variable[+]=value ...]
#                     [TIMEOUT seconds]
#                     [TYPE Boost|CppUnit])
#
# Special version of elements_add_executable which automatically adds the
# dependency on CppUnit and declares the test to CTest (add_test).
# The WORKING_DIRECTORY option can be passed if the command needs to be
# run from a fixed directory.
# If special environment settings are needed, they can be specified in the
# section ENVIRONMENT as <var>=<value> or <var>+=<value>, where the second
# format prepends the value to the PATH-like variable.
# The default TYPE is CppUnit and Boost can also be specified.
#-----------------------------------------------------------------------------
function( add_unit_test name )

  CMAKE_PARSE_ARGUMENTS(${name}_UNIT_TEST "" "EXECUTABLE;TYPE;TIMEOUT;WORKING_DIRECTORY" "ENVIRONMENT;LABELS" ${ARGN})

  common_add_build(${${name}_UNIT_TEST_UNPARSED_ARGUMENTS})

  if(NOT ${name}_UNIT_TEST_TYPE)
    set(${name}_UNIT_TEST_TYPE None)
  endif()

  if(NOT ${name}_UNIT_TEST_WORKING_DIRECTORY)
    set(${name}_UNIT_TEST_WORKING_DIRECTORY .)
  endif()

  if(NOT ${name}_UNIT_TEST_EXECUTABLE)
      set(${name}_UNIT_TEST_EXECUTABLE ${name})
    endif()

  set(executable ${${name}_UNIT_TEST_EXECUTABLE})

  # elements_get_package_name(package)
  get_filename_component(package ${CMAKE_CURRENT_SOURCE_DIR} NAME)

  if(NOT ${${name}_UNIT_TEST_TYPE} STREQUAL "None")
    if (${${name}_UNIT_TEST_TYPE} STREQUAL "Boost")
      find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    else()
      find_package(${${name}_UNIT_TEST_TYPE} QUIET REQUIRED)
    endif()
    if (NOT TARGET ${package}_tests_dir)
      add_custom_target(${package}_tests_dir
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/unittests
                        COMMENT "Generating The ${package}/unittests directory" VERBATIM)
    endif()
    set(testmain_file ${CMAKE_CURRENT_BINARY_DIR}/unittests/${${name}_UNIT_TEST_TYPE}TestMain.cpp)
    set_source_files_properties(${testmain_file} PROPERTIES GENERATED TRUE)
    if(NOT TARGET ${package}_${${name}_UNIT_TEST_TYPE}TestMain)
      add_custom_target(${package}_${${name}_UNIT_TEST_TYPE}TestMain
                        COMMAND ${${${name}_UNIT_TEST_TYPE}_testmain_cmd} --quiet ${package} ${testmain_file}
                        DEPENDS ${package}_tests_dir
                        COMMENT "Generating the ${package} ${${name}_UNIT_TEST_TYPE}TestMain.cpp" VERBATIM)
    endif()
    set(srcs ${srcs} ${testmain_file})

    message( STATUS "srcs = ${srcs}" )
    message( STATUS "exec = ${executable}" )
    # elements_add_executable(${executable} ${srcs}
    #                           LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${${name}_UNIT_TEST_TYPE}
    #                           INCLUDE_DIRS ${ARG_INCLUDE_DIRS} ${${name}_UNIT_TEST_TYPE})
    #                           add_dependencies(${executable} ${package}_${${name}_UNIT_TEST_TYPE}TestMain)
    add_executable( ${executable} ${srcs})
    #target_link_libraries(${executable} ${ARG_LINK_LIBRARIES} ${${name}_UNIT_TEST_TYPE} )
    target_link_libraries(${executable} ${ARG_LINK_LIBRARIES} )
    target_include_directories( ${executable} PUBLIC src )
    add_dependencies(${executable} ${package}_${${name}_UNIT_TEST_TYPE}TestMain)

  else()
    #elements_add_executable(${executable} ${srcs}
    #                        LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
    #                        INCLUDE_DIRS ${ARG_INCLUDE_DIRS})
  endif()

  ##get_target_property(exec_suffix ${executable} SUFFIX)
  ##if(NOT exec_suffix)
  ##  set(exec_suffix)
  ##endif()

  foreach(var ${${name}_UNIT_TEST_ENVIRONMENT})
    string(FIND ${var} "+=" is_prepend)
    if(NOT is_prepend LESS 0)
      # the argument contains +=
      string(REPLACE "+=" "=" var ${var})
      set(extra_env ${extra_env} -p ${var})
    else()
      set(extra_env ${extra_env} -s ${var})
    endif()
  endforeach()

  add_test(NAME ${package}.${name}
             WORKING_DIRECTORY ${${name}_UNIT_TEST_WORKING_DIRECTORY}
             COMMAND ${env_cmd} ${extra_env} --xml ${env_xml}
             ${executable}${exec_suffix})

  set_property(TEST ${package}.${name} APPEND PROPERTY LABELS UnitTest ${package} Binary)
  if(NOT ${${name}_UNIT_TEST_TYPE} STREQUAL "None")
    set_property(TEST ${package}.${name} APPEND PROPERTY LABELS ${${name}_UNIT_TEST_TYPE})
  endif()

  foreach(t ${${name}_UNIT_TEST_LABELS})
    set_property(TEST ${package}.${name} APPEND PROPERTY LABELS ${t})
  endforeach()

  if(${name}_UNIT_TEST_TIMEOUT)
    set_property(TEST ${package}.${name} PROPERTY TIMEOUT ${${name}_UNIT_TEST_TIMEOUT})
  endif()

endfunction()
