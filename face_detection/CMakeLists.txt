project( face_dec )
cmake_minimum_required( VERSION 2.8 )
add_definitions(-std=c++11)





file( GLOB SRCS *.c *.cpp *.cc *.h *.hpp)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")
add_executable( ${PROJECT_NAME} ${SRCS} )
include_directories("spdlog/include")
include_directories("dlib")


find_package(dlib REQUIRED)
include_directories(${DLIB_INCLUDE_DIRECTORY})
target_link_libraries(face_dec  libdlib.a X11 pthread)



