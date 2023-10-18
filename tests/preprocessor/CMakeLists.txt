add_executable(num_literals_test number_literals.cpp)
target_link_libraries(num_literals_test PRIVATE src)

add_test(
  NAME number_literals_test
  COMMAND $<TARGET_FILE:num_literals_test>
)

add_executable(str_literals_test string_literals.cpp)
target_link_libraries(str_literals_test PRIVATE src)

add_test(
  NAME string_literals_test
  COMMAND $<TARGET_FILE:str_literals_test>
)

add_executable(char_constants_test char_constants.cpp)
target_link_libraries(char_constants_test PRIVATE src)

add_test(
  NAME char_constants_test
  COMMAND $<TARGET_FILE:char_constants_test>
)

add_executable(identifiers_test identifiers.cpp)
target_link_libraries(identifiers_test PRIVATE src)

add_test(
  NAME identifiers_test
  COMMAND $<TARGET_FILE:identifiers_test>
)

add_executable(macro_test macro.cpp)
target_link_libraries(macro_test PRIVATE src)

#add_executable(overall_test overall.cpp)
#target_link_libraries(overall_test PRIVATE src)
#
#add_test(
#  NAME overall_test
#  COMMAND $<TARGET_FILE:overall_test>
#)
