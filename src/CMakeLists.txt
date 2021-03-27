find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
pkg_check_modules(OPENCV REQUIRED opencv)

find_library(BBQUE_RTLIB_LIBRARY bbque_rtlib
        HINTS ${BOSP_SYSROOT}
        PATHS ${BOSP_SYSROOT}
        PATH_SUFFIXES lib/bbque
)

# Check for required libs and packages (headers + lib)
find_package(Boost 1.65.0 REQUIRED program_options
        PATHS ${BOSP_SYSROOT})
message (STATUS "Boost libraries: ${Boost_LIBRARIES}")
message (STATUS "Boost include  : ${Boost_INCLUDE_DIRS}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(peopledetect_SOURCES main.cpp peopledetect.h peopledetect.cpp)
add_executable(peopledetect ${peopledetect_SOURCES})
target_link_libraries(peopledetect
    ${OPENCV_LIBRARIES}
    ${BBQUE_RTLIB_LIBRARY}
    ${Boost_LIBRARIES}
    )

# Use link path ad RPATH
set_property(TARGET peopledetect PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install (TARGETS peopledetect RUNTIME
        DESTINATION ${CMAKE_INSTALL_PREFIX}/usr/bin)

