include(AddGoogleTest)

add_executable(jlinkdb_test jlinkdb_test.cc)
target_link_libraries(jlinkdb_test jlinkdb gtest_main)
add_test(jlinkdb_test jlinkdb_test)
