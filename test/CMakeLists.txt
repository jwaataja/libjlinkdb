include(AddGoogleTest)

add_executable(libjlinkdb_test libjlinkdb_test.cc)
target_link_libraries(libjlinkdb_test libjlinkdb gtest_main)
add_test(libjlinkdb_test libjlinkdb_test)
