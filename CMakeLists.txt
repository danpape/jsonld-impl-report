cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Version settings

set(JSONLD_IMPL_REPORT_VERSION_MAJOR 1)
set(JSONLD_IMPL_REPORT_VERSION_MINOR 0)
set(JSONLD_IMPL_REPORT_VERSION_PATCH 0)
set(JSONLD_IMPL_REPORT_VERSION ${JSONLD_IMPL_REPORT_VERSION_MAJOR}.${JSONLD_IMPL_REPORT_VERSION_MINOR}.${JSONLD_IMPL_REPORT_VERSION_PATCH} CACHE STRING "" FORCE)

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(installDir ${CMAKE_CURRENT_BINARY_DIR}/install)
else()
  set(installDir ${CMAKE_INSTALL_PREFIX})
endif()
message(STATUS "installDir        : " ${installDir})

# Project

project(jsonld-impl-report VERSION ${JSONLD_IMPL_REPORT_VERSION}
        DESCRIPTION "JSON-LD Implementation test results reporting tool"
        HOMEPAGE_URL "https://github.com/dcdpr/jsonld-impl-report"
        LANGUAGES C CXX )


# Super build...
include(ExternalProject)

# Add subdirectories for external dependencies

ExternalProject_Add(uriparser
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/external/uriparser
      INSTALL_DIR ${installDir}
      CMAKE_ARGS  -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
                  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                  -DURIPARSER_BUILD_DOCS=OFF
                  -DURIPARSER_BUILD_TESTS=OFF
                  -DURIPARSER_BUILD_TOOLS=OFF
                  -DURIPARSER_BUILD_CHAR=ON
                  -DURIPARSER_BUILD_WCHAR_T=ON
                  -DURIPARSER_ENABLE_INSTALL=ON
  )

# Add local subdirectories

ExternalProject_Add(jsonld-impl-report-app
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/jsonld-impl-report
      INSTALL_DIR ${installDir}
      CMAKE_ARGS  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                  -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
                  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                  -DJSONLD_IMPL_REPORT_VERSION=${JSONLD_IMPL_REPORT_VERSION}
      INSTALL_COMMAND     ""
      DEPENDS     uriparser
  )

