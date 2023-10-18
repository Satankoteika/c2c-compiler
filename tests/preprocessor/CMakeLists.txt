add_executable(preprocessor_test test.cpp)
target_link_libraries(preprocessor_test PRIVATE src)

add_test(
  NAME preprocessor_test
  COMMAND $<TARGET_FILE:preprocessor_test>
)
