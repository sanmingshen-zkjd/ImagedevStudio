# Centralized output for component/plugin targets so executables and plugins
# are built into a single predictable directory.
set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins)

# For multi-config generators (VS/Xcode), set per-config output folders
if (CMAKE_CONFIGURATION_TYPES)
    foreach(cfg IN LISTS CMAKE_CONFIGURATION_TYPES)
        string(TOUPPER "${cfg}" cfgUpper)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgUpper} "${PLUGIN_OUTPUT_DIR}/${cfg}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfgUpper} "${PLUGIN_OUTPUT_DIR}/${cfg}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfgUpper} "${PLUGIN_OUTPUT_DIR}/${cfg}")
    endforeach()
else()
    # Single-config generators (Ninja, Makefiles)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}/$<CONFIG>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}/$<CONFIG>")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}/$<CONFIG>")
endif()

macro(dirlist result currentDir)
    file(GLOB subdirs RELATIVE ${currentDir} ${currentDir}/*)
    set(dirlist "")
    foreach(entry ${subdirs})
        set(currentSubDir "${CMAKE_CURRENT_SOURCE_DIR}/${entry}" CACHE STRING "" FORCE)
        if(IS_DIRECTORY ${currentSubDir} AND EXISTS "${currentSubDir}/CMakeLists.txt")
            list(APPEND dirlist ${entry})
        endif()
        set(currentDir "")
    endforeach()
    set(${result} ${dirlist})
endmacro()

macro(plugin_list result)
    file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
    foreach(entry ${subdirs})
        set(currentSubDir "${CMAKE_CURRENT_SOURCE_DIR}/${entry}" CACHE STRING "" FORCE)
        if(IS_DIRECTORY ${currentSubDir} AND EXISTS "${currentSubDir}/${entry}plugin.h")
            list(APPEND ${result} "${entry}")
        endif()
    endforeach()
endmacro()

macro(get_plugin_type plugin result section)
    set(pluginName "" CACHE STRING "" FORCE)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/${plugin}plugin.h")
        file(STRINGS ${plugin}/${plugin}plugin.h
            test
            REGEX "^(struct)|(class).*"
        )
    else()
        set(test "")
    endif()

    string(REPLACE "struct" "" test "${test}")
    string(REPLACE "class" "" test "${test}")
    string(REPLACE "{" "" test "${test}")
    string(STRIP "${test}" test)
    message(STATUS "Found Plugin: ${test}")
    set(${result} ${test})

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/${plugin}plugin.h")
        file(STRINGS ${plugin}/${plugin}plugin.h
            secTest
            REGEX "PluginType"
        )
    else()
        set(secTest "")
    endif()

    string(REPLACE "PluginType" "" secTest "${secTest}")
    string(REPLACE "using" "" secTest "${secTest}")
    string(REPLACE "=" "" secTest "${secTest}")
    string(REPLACE ";" "" secTest "${secTest}")
    string(STRIP "${secTest}" secTest)
    set(${section} ${secTest})
endmacro()

plugin_list(plugins)
set(PLUGIN_LIBRARIES ${plugins} CACHE INTERNAL "Libraries to link against for plugins")
set(PLUGINS_IMPL "")
set(PLUGIN_INCLUDES "")
set(SECTIONS "")

foreach(component ${plugins})
    set(PLUGIN_INCLUDES "${PLUGIN_INCLUDES}#include <${CMAKE_CURRENT_SOURCE_DIR}/${component}/${component}plugin.h>\n")
    get_plugin_type(${component} pluginType sectionType)

    # Normalize and strip results
    string(STRIP "${pluginType}" _tmp_plugin)
    string(STRIP "${sectionType}" _tmp_section)

    # Append plugin type only if not already present (defensive against differing whitespace/casing)
    if(NOT _tmp_plugin STREQUAL "")
        list(FIND PLUGINS_IMPL "${_tmp_plugin}" _found_plugin)
        if(_found_plugin EQUAL -1)
            list(APPEND PLUGINS_IMPL "${_tmp_plugin}")
        else()
            # already present — skip
        endif()
    endif()

    # Append section only if not already present
    if(NOT _tmp_section STREQUAL "")
        list(FIND SECTIONS "${_tmp_section}" _found_section)
        if(_found_section EQUAL -1)
            list(APPEND SECTIONS "${_tmp_section}")
        else()
            # already present — skip
        endif()
    endif()
endforeach()

# Final dedupe pass (extra safety)
list(REMOVE_DUPLICATES SECTIONS)
list(REMOVE_DUPLICATES PLUGINS_IMPL)

# Show debug message of computed lists
message(STATUS "Computed plugin classes: ${PLUGINS_IMPL}")
message(STATUS "Computed sections: ${SECTIONS}")

# Build comma-separated PLUGINS_IMPL_STR without trailing comma, skipping empty entries
set(PLUGINS_IMPL_MSG "")
set(_first TRUE)
foreach(plugin_str ${PLUGINS_IMPL})
    string(STRIP "${plugin_str}" plugin_str)
    if(NOT plugin_str STREQUAL "")
        if(_first)
            set(PLUGINS_IMPL_MSG "${plugin_str}")
            set(_first FALSE)
        else()
            set(PLUGINS_IMPL_MSG "${PLUGINS_IMPL_MSG}, ${plugin_str}")
        endif()
    endif()
endforeach()
set(PLUGINS_IMPL_STR "${PLUGINS_IMPL_MSG}")

# Build comma-separated SECTIONS_IMPL without trailing comma
set(SECTIONS_IMPL_MSG "")
set(_first_sections TRUE)
foreach(sections_str ${SECTIONS})
    string(STRIP "${sections_str}" sections_str)
    if(NOT sections_str STREQUAL "")
        if(_first_sections)
            set(SECTIONS_IMPL_MSG "${sections_str}")
            set(_first_sections FALSE)
        else()
            set(SECTIONS_IMPL_MSG "${SECTIONS_IMPL_MSG}, ${sections_str}")
        endif()
    endif()
endforeach()
set(SECTIONS_IMPL "${SECTIONS_IMPL_MSG}")

# SANITIZE: remove any accidental backslashes introduced by regex/expansion
string(REPLACE "\\" "" PLUGINS_IMPL_STR "${PLUGINS_IMPL_STR}")
string(REPLACE "\\" "" SECTIONS_IMPL "${SECTIONS_IMPL}")

# Final defensive trim
string(STRIP "${PLUGINS_IMPL_STR}" PLUGINS_IMPL_STR)
string(STRIP "${SECTIONS_IMPL}" SECTIONS_IMPL)

configure_file(${CMAKE_SOURCE_DIR}/src/common/plugins.hpp.in
    ${CMAKE_BINARY_DIR}/plugins.hpp)

dirlist(components ${CMAKE_CURRENT_SOURCE_DIR})
foreach(component ${components})
    add_subdirectory(${component})
endforeach()