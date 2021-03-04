add_library(heap_sort sort.h sort.cpp)
set_target_properties(heap_sort PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test_heap_sort testHeapSort.cpp)

target_link_libraries(test_heap_sort heap_sort)
