# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_Camero_VRPN_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED Camero_VRPN_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(Camero_VRPN_FOUND FALSE)
  elseif(NOT Camero_VRPN_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(Camero_VRPN_FOUND FALSE)
  endif()
  return()
endif()
set(_Camero_VRPN_CONFIG_INCLUDED TRUE)

# output package information
if(NOT Camero_VRPN_FIND_QUIETLY)
  message(STATUS "Found Camero_VRPN: 0.0.0 (${Camero_VRPN_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'Camero_VRPN' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${Camero_VRPN_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(Camero_VRPN_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${Camero_VRPN_DIR}/${_extra}")
endforeach()
