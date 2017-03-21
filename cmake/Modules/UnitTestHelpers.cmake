# Unit test dedfinitions

# written by: Oliver Cordes 2017-03-16
# changed by: Oliver Cordes 2017-03-20


# enable testing mode
enable_testing()
include(CTest)

# paths where to locate scripts and executables
set(binary_paths)
foreach(modp ${CMAKE_MODULE_PATH})
  if(EXISTS ${modp}/scripts)
    set(binary_paths ${binary_paths} ${modp}/scripts)
endif()
endforeach()

list(REMOVE_DUPLICATES binary_paths)

#search the Boost-Testprogram Main-Template generator script
find_program(Boost_testmain_cmd createBoostTestMain.py HINTS ${binary_paths})

find_program(env_cmd env.py HINTS ${binary_paths})
set(env_cmd ${PYTHON_EXECUTABLE} ${env_cmd})

set(env_xml ${CMAKE_BINARY_DIR}/${project}BuildEnvironment.xml
      CACHE STRING "path to the XML file for the environment to be used in building and testing")


# stolen from EuclidEnvironment and modified for arctic

#-------------------------------------------------------------------------------
# arctic_generate_env_conf(filename <env description>)
#
# Generate the XML file describing the changes to the environment required by
# this project.
#-------------------------------------------------------------------------------
function(arctic_generate_env_conf filename)
  set(data "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<env:config xmlns:env=\"EnvSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"EnvSchema EnvSchema.xsd \">\n")

  # variables that need to be used to make the environment relative
  #set(root_vars SGS_releases SGS_external)
  set(root_vars arctic_releases )
  foreach(root_var ${root_vars})
    set(data "${data}  <env:default variable=\"${root_var}\">${${root_var}}</env:default>\n")
  endforeach()

  # include inherited environments
  # (note: it's important that the full search path is ready before we start including)

  foreach(other_project ${used_elements_projects})
    set(data "${data}  <env:search_path>${${other_project}_DIR}</env:search_path>\n")
  endforeach()
  foreach(other_project ${used_elements_projects})
    set(data "${data}  <env:include>${other_project}Environment.xml</env:include>\n")
  endforeach()


  set(commands ${ARGN})
  #message(STATUS "start - ${commands}")
  while(commands)
   #message(STATUS "iter - ${commands}")
   _env_conf_pop_instruction(instr commands)
   # ensure that the variables in the value are not expanded when passing the arguments
   string(REPLACE "\$" "\\\$" instr "${instr}")
   _env_line(${instr} ln)
   set(data "${data}  ${ln}\n")
  endwhile()
  set(data "${data}</env:config>\n")

  get_filename_component(fn ${filename} NAME)
  message(STATUS "Generating ${fn}")
  file(WRITE ${filename} "${data}")
endfunction()

arctic_generate_env_conf(${env_xml} ${project_build_environment})

#-------------------------------------------------------------------------------
# resolve_link_libraries(variable lib_or_package1 lib_or_package2 ...)
#
# Translate the package names in a list of link library options into the
# corresponding library options.
# Example:
#
#  find_package(Boost COMPONENTS filesystem regex)
#  find_package(ROOT COMPONENTS RIO)
#  resolve_link_libraries(LIBS Boost ROOT)
#  ...
#  target_link_libraries(XYZ ${LIBS})
#
# Note: this function is more useful in wrappers to add_library etc, like
#       elements_add_library
#-------------------------------------------------------------------------------
function(resolve_link_libraries variable)
  #message(STATUS "elements_resolve_link_libraries input: ${ARGN}")
  set(collected)
  set(to_be_resolved)
  foreach(package ${ARGN})
    # check if it is an actual library or a target first
    if(TARGET ${package})
      #message(STATUS "${package} is a TARGET")
      set(collected ${collected} ${package})
      get_target_property(libs ${package} REQUIRED_LIBRARIES)
      if(libs)
        set(to_be_resolved ${to_be_resolved} ${libs})
      endif()
    elseif(EXISTS ${package}) # it's a real file
      #message(STATUS "${package} is a FILE")
      set(collected ${collected} ${package})
    else()
      # it must be an available package
      string(TOUPPER ${package} _pack_upper)
      # The case of CMAKE_DL_LIBS is more special than others
      if(${_pack_upper}_FOUND OR ${package}_FOUND)
        # Handle some special cases first, then try for PACKAGE_LIBRARIES
        # otherwise fall back on Package_LIBRARIES.
        if(${package} STREQUAL PythonLibs)
          set(collected ${collected} ${PYTHON_LIBRARIES})
        elseif(${_pack_upper}_LIBRARIES)
          set(collected ${collected} ${${_pack_upper}_LIBRARIES})
        else()
          set(collected ${collected} ${${package}_LIBRARIES})
          endif()
        else()
          # if it's not a package, we just add it as it is... there are a lot of special cases
          set(collected ${collected} ${package})
        endif()
      endif()
    endforeach()
    ##_elements_strip_build_type_libs(to_be_resolved)
    ##if(to_be_resolved)
    ##  elements_resolve_link_libraries(to_be_resolved ${to_be_resolved})
    ##  set(collected ${collected} ${to_be_resolved})
    ##endif()
    ###message(STATUS "elements_resolve_link_libraries collected: ${collected}")
    ##_elements_strip_build_type_libs(collected)
    ###message(STATUS "elements_resolve_link_libraries output: ${collected}")
    set(${variable} ${collected} PARENT_SCOPE)
  endfunction()


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
# include_package_directories(Package1 [Package2 ...])
#
# Add the include directories of each package to the include directories.
#-------------------------------------------------------------------------------
function(include_package_directories)
  #message(STATUS "include_package_directories(${ARGN})")
  foreach(package ${ARGN})
    # we need to ensure that the user can call this function also for directories
    if(TARGET ${package})
      get_target_property(to_incl ${package} SOURCE_DIR)
      if(to_incl)
        #message(STATUS "include_package_directories1 include_directories(${to_incl})")
        include_directories(${to_incl})
      endif()
    elseif(IS_ABSOLUTE ${package} AND IS_DIRECTORY ${package})
      #message(STATUS "include_package_directories2 include_directories(${package})")
      include_directories(${package})
    elseif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${package})
      #message(STATUS "include_package_directories3 include_directories(${package})")
      include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${package})
    elseif(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${package}) # package can be the name of a subdir
      #message(STATUS "include_package_directories4 include_directories(${package})")
      include_directories(${CMAKE_SOURCE_DIR}/${package})
      else()
        # ensure that the current directory knows about the package
        find_package(${package} QUIET)
        set(to_incl)
        string(TOUPPER ${package} _pack_upper)
        if(${_pack_upper}_FOUND OR ${package}_FOUND)
          # Handle some special cases first, then try for package uppercase (DIRS and DIR)
          # If the package is found, add INCLUDE_DIRS or (if not defined) INCLUDE_DIR.
          # If none of the two is defined, do not add anything.
          if(${package} STREQUAL PythonLibs)
            set(to_incl PYTHON_INCLUDE_DIRS)
          elseif(${_pack_upper}_INCLUDE_DIRS)
            set(to_incl ${_pack_upper}_INCLUDE_DIRS)
          elseif(${_pack_upper}_INCLUDE_DIR)
            set(to_incl ${_pack_upper}_INCLUDE_DIR)
          elseif(${package}_INCLUDE_DIRS)
            set(to_incl ${package}_INCLUDE_DIRS)
          endif()
          # Include the directories
          #message(STATUS "include_package_directories5 include_directories(${${to_incl}})")
          include_directories(${${to_incl}})
        endif()
      endif()
    endforeach()
endfunction()

#-------------------------------------------------------------------------------
# arctic_common_add_build(sources...
#                 LINK_LIBRARIES library1 package2 ...
#                 INCLUDE_DIRS dir1 package2 ...)
#
# Internal. Helper macro to factor out the common code to configure a buildable
# target (library, module, dictionary...)
#-------------------------------------------------------------------------------
macro(arctic_common_add_build)
  CMAKE_PARSE_ARGUMENTS(ARG "" "" "LIBRARIES;LINK_LIBRARIES;INCLUDE_DIRS" ${ARGN})
  # obsolete option
  if(ARG_LIBRARIES)
    message(WARNING "Deprecated option 'LIBRARIES', use 'LINK_LIBRARIES' instead")
    set(ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${ARG_LIBRARIES})
  endif()

  resolve_link_libraries(ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES})

  # find the sources
  expand_sources(srcs ${ARG_UNPARSED_ARGUMENTS})

  #message(STATUS "elements_common_add_build ${ARG_LINK_LIBRARIES}")
  # get the inherited include directories
  get_required_include_dirs(ARG_INCLUDE_DIRS ${ARG_LINK_LIBRARIES})
  #message(STATUS "elements_common_add_build ${ARG_LINK_LIBRARIES}")

  #message(STATUS "elements_common_add_build ${ARG_INCLUDE_DIRS}")
  # add the package includes to the current list
  include_package_directories(${ARG_INCLUDE_DIRS})

  #message(STATUS "elements_common_add_build ARG_LINK_LIBRARIES ${ARG_LINK_LIBRARIES}")
  # get the library dirs required to get the libraries we use
  get_required_library_dirs(lib_path ${ARG_LINK_LIBRARIES})
  set_property(GLOBAL APPEND PROPERTY LIBRARY_PATH ${lib_path})

endmacro()

#---------------------------------------------------------------------------------------------------
# arctic_add_executable(<name>
#                      source1 source2 ...
#                      LINK_LIBRARIES library1 library2 ...
#                      INCLUDE_DIRS dir1 package2 ...)
#
# Extension of standard CMake 'add_executable' command.
# Create a library from the specified sources (glob patterns are allowed), linking
# it with the libraries specified and adding the include directories to the search path.
#---------------------------------------------------------------------------------------------------
function(arctic_add_executable executable)
  arctic_common_add_build(${ARGN})

  add_executable(${executable} ${srcs})
  target_link_libraries(${executable} ${ARG_LINK_LIBRARIES})
  ##_elements_detach_debinfo(${executable})

  ##if (ELEMENTS_USE_EXE_SUFFIX)
  ##  set_target_properties(${executable} PROPERTIES SUFFIX .exe)
  ##endif()

  ###----Installation details-------------------------------------------------------
  ##install(TARGETS ${executable} EXPORT ${CMAKE_PROJECT_NAME}Exports RUNTIME DESTINATION bin OPTIONAL)
  ##install(EXPORT ${CMAKE_PROJECT_NAME}Exports DESTINATION cmake OPTIONAL)
  ##  elements_export(EXECUTABLE ${executable})
  ##  set_property(GLOBAL APPEND PROPERTY REGULAR_BIN_OBJECTS ${executable})
  ##  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_CMAKE TRUE)
  endfunction()

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

  arctic_common_add_build(${${name}_UNIT_TEST_UNPARSED_ARGUMENTS})

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
      find_package(Boost COMPONENTS unit_test_framework QUIET REQUIRED)
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

    arctic_add_executable(${executable} ${srcs}
                               LINK_LIBRARIES ${ARG_LINK_LIBRARIES} ${${name}_UNIT_TEST_TYPE}
                               INCLUDE_DIRS ${ARG_INCLUDE_DIRS} ${${name}_UNIT_TEST_TYPE})
                               add_dependencies(${executable} ${package}_${${name}_UNIT_TEST_TYPE}TestMain)
    add_dependencies(${executable} ${package}_${${name}_UNIT_TEST_TYPE}TestMain)

  else()
    arctic_add_executable(${executable} ${srcs}
                            LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
                            INCLUDE_DIRS ${ARG_INCLUDE_DIRS})
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
             ${CMAKE_CURRENT_BINARY_DIR}/${executable}${exec_suffix})

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
