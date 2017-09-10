# Install script for directory: /home/zhao/projects/PropertyManager/src/simgear/xml

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/LYRASim/simgear/xml" TYPE FILE FILES
    "/home/zhao/projects/PropertyManager/src/simgear/xml/easyxml.hxx"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/ascii.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/asciitab.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/expat.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/expat_external.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/iasciitab.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/internal.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/latin1tab.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/macconfig.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/nametab.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/utf8tab.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/winconfig.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/xmlrole.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/xmltok.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/xmltok_impl.h"
    "/home/zhao/projects/PropertyManager/src/simgear/xml/expat_config.h"
    )
endif()

