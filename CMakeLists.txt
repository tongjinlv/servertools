project(servertools)
cmake_minimum_required( VERSION 2.8 )
add_definitions(-std=c++11)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/INSTALL)

add_subdirectory(lib)

include_directories(${CMAKE_BINARY_DIR}/INSTALL/include)
#find_library(LIBHELLO_PATH dlib ${CMAKE_BINARY_DIR}/INSTALL/lib)

MESSAGE("zzzzzzzzzzzzzzzzzzzzzzzzzz${LIBHELLO_PATH}${dlib}")
MESSAGE("zzzzzzzzzzzzzzzzzzzzzzzzzz ${DLIB_LIBRARY_DIRS}")

#file( GLOB SRCS *.c *.cpp *.cc *.h *.hpp)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")
#add_executable( ${PROJECT_NAME} ${SRCS} )
#include_directories("spdlog/include")
#include_directories("dlib")




