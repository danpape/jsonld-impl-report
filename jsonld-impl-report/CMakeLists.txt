cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(jsonld-impl-report-app VERSION ${JSONLD_IMPL_REPORT_VERSION}
        LANGUAGES C CXX )

#include(GNUInstallDirs)

find_package(uriparser 0.9.5 CONFIG REQUIRED char wchar_t)

#
# Implementation Report
#
add_executable(
    implementation-report
    main.cpp
    CommandRunner.h
    CommandRunner.cpp
    TestRunner.h
    TestRunner.cpp
    ReportWriter.h
    ReportWriter.cpp
    EarlFormatter.h
    EarlFormatter.cpp
    ConfigReader.h
    ConfigReader.cpp
    RdfData.h
    RdfData.cpp
    RdfDataBuilder.h
    RdfDataBuilder.cpp
    TestResult.h
    Uri.h
    Uri.cpp
    UriParser.h
    UriParser.cpp
)
target_compile_features(
    implementation-report
    PRIVATE
    cxx_std_14
)

set_target_properties(
    implementation-report
    PROPERTIES
    CXX_EXTENSIONS OFF
)
target_include_directories(implementation-report
        PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..> # this makes include statements like '#include "jsonld-impl-report/json.hpp"' work
        )


target_link_libraries(
        implementation-report
        PUBLIC uriparser::uriparser
)

#  install(
#    TARGETS implementation-report
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#  )

