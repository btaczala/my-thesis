project(Thesis-application)
cmake_minimum_required(VERSION 2.6)
set( CMAKE_MODULE_PATH    ${CMAKE_MODULE_PATH}
                          ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules )
find_package(Qt4 REQUIRED)
find_package(Boost REQUIRED 1.37.0)
find_package( Qwt5 REQUIRED)
include_directories("3rdParty")
add_subdirectory(3rdParty/muparser)
add_subdirectory(src)

