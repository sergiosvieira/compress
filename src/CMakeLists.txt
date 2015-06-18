_add_subdirectories(${CMAKE_CURRENT_LIST_DIR})
add_executable(zipper main.cpp $ENV{PUBLIC_HEADERS})
target_link_libraries(zipper zipManager)
target_link_libraries(zipper utils)
