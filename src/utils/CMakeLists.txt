file(GLOB SOURCES "*.cpp")
_add_subdirectories(${CMAKE_CURRENT_LIST_DIR})
add_library(zipperUtils ${SOURCES} $ENV{PUBLIC_HEADERS})