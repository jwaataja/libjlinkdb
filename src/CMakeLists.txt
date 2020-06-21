add_library(
	libjlinkdb
	STATIC
	link_entry.cc
	link_database.cc
	string_utils.cc
	jlinkdb_error.cc)

target_compile_features(libjlinkdb PUBLIC cxx_std_11)
set_target_properties(libjlinkdb PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(libjlinkdb PRIVATE -Werror -Wall -Wextra)

target_include_directories(libjlinkdb
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
	${GLIBMM_INCLUDE_DIRS}
	${LIBSIGCPP_INCLUDE_DIRS})

target_link_libraries(libjlinkdb
	PUBLIC
	${GLIBMM_LIBRARIES}
	${LIBSIGCPP_LIBRARIES}
	PRIVATE
	LUrlParser)

target_link_libraries(libjlinkdb PUBLIC nlohmann_json::nlohmann_json)

add_subdirectory(query)
