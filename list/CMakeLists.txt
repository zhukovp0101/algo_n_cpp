add_library(list list.h)
set_target_properties(list PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test_list testList.cpp)

target_link_libraries(test_list list) 
