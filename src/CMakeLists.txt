add_library(jlinkdb STATIC jlinkdb.cc)

target_compile_features(jlinkdb PUBLIC cxx_std_11)
set_target_properties(jlinkdb PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(jlinkdb PRIVATE -Werror -Wall -Wextra)

target_include_directories(jlinkdb
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
	${GLIBMM_INCLUDE_DIRS}
	${LIBXMLPP_INCLUDE_DIRS})

target_link_libraries(jlinkdb
	PUBLIC
	${GLIBMM_LIBRARIES}
	${LIBXMLPP_LIBRARIES})

