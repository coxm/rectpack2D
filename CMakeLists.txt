cmake_minimum_required(VERSION 3.7)
project(rectpack2D VERSION 0.0.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(
  rectpack2D_BUILD_EXAMPLE
  "Build the rectpack2D example program"
  ${BUILD_TESTING})


add_library(rectpack2D INTERFACE)
target_include_directories(rectpack2D INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
add_library(rectpack2D::rectpack2D ALIAS rectpack2D)


configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/rectpack2DConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/rectpack2DConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/rectpack2D"
)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/rectpack2DConfigVersion.cmake"
  COMPATIBILITY AnyNewerVersion
)
configure_file(rectpack2D.pc.in rectpack2D.pc @ONLY)


install(DIRECTORY ${PROJECT_SOURCE_DIR}/include
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h")
install(
  FILES
    "${PROJECT_BINARY_DIR}/rectpack2DConfig.cmake"
    "${PROJECT_BINARY_DIR}/rectpack2DConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/rectpack2D")
install(
  FILES "${PROJECT_BINARY_DIR}/rectpack2D.pc"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig")
install(TARGETS rectpack2D
  EXPORT rectpack2D-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT rectpack2D-targets
  FILE rectpack2DTargets.cmake
  NAMESPACE rectpack2D::
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/rectpack2D")
export(EXPORT rectpack2D-targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/rectpack2DTargets.cmake"
  NAMESPACE rectpack2D::)
export(PACKAGE rectpack2D)


if(rectpack2D_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()
