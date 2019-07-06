cmake_minimum_required(VERSION 3.14)

project(jlinkdb VERSION 0.1.0 LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXMLPP REQUIRED libxml++-3.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)

add_library(jlinkdb STATIC jlinkdb.cc jlinkdb.hh jlinkdb_impl.hh)

target_compile_features(jlinkdb PUBLIC cxx_std_11)
set_target_properties(jlinkdb PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(jlinkdb PRIVATE -Werror -Wall -Wextra)

target_include_directories(jlinkdb
	PUBLIC
	${GLIBMM_LIBRARY_DIRS}
	${LIBXMLPP_LIBRARY_DIRS})

target_link_libraries(jlinkdb
	PUBLIC
	${GLIBMM_LIBRARY_DIRS}
	${LIBXMLPP_LIBRARY_DIRS})
