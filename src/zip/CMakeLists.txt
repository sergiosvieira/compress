file(GLOB SOURCES "*.cpp")
_add_subdirectories(${CMAKE_CURRENT_LIST_DIR})
add_library(zipManager ${SOURCES} $ENV{PUBLIC_HEADERS})
target_link_libraries(zipManager ${LibArchive_LIBRARIES})
