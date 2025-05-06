cmake_minimum_required(VERSION 3.28)
project(PdbParser LANGUAGES C CXX DESCRIPTION "Lightweight microsoft pdb parser")

include("${CMAKE_CURRENT_LIST_DIR}/pdbparser-config.cmake")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/PdbParserConfigVersion.cmake"
        VERSION 1.0.1
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/PdbParserConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/PdbParserConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/CMake/PdbParser
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/PdbParserConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/PdbParserConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/CMake/PdbParser
)
