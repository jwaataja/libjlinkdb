cmake_minimum_required(VERSION 3.14)

project(libjlinkdb VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXMLPP REQUIRED libxml++-3.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(LIBSIGCPP REQUIRED sigc++-2.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
