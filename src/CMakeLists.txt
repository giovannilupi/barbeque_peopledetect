find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
pkg_check_modules(OPENCV REQUIRED opencv)

add_executable(peopledetect main.cpp peopledetect.h peopledetect.cpp)
target_link_libraries(peopledetect ${OPENCV_LIBRARIES})
