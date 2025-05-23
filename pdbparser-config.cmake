if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binary")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binary")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Binary")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/Sources/*.cpp")
add_library(PdbParser SHARED ${SOURCES})

target_include_directories(PdbParser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>
        $<INSTALL_INTERFACE:include>
)

add_library(PdbParser::PdbParser ALIAS PdbParser)

include(GNUInstallDirs)

install(TARGETS PdbParser
        EXPORT PdbParserTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT PdbParserTargets
        FILE PdbParserTargets.cmake
        NAMESPACE PdbParser::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/CMake/PdbParser
)

include_directories("${CMAKE_CURRENT_LIST_DIR}/Sources")
