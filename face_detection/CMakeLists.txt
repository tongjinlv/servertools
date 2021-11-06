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



include(ExternalProject)
set(DLIB_SRC_PATH "dlib")
ExternalProject_Add(dlib
    SOURCE_DIR dlib
    GIT_REPOSITORY https://github.com/davisking/dlib.git
    GIT_TAG 7f6746e7cdd0de3de261bd5c018a933612d325f0
    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DTOOLS_DIRECTORY=${TOOLS_DIRECTORY}
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}
        -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        -DSPDLOG_BUILD_TESTING=OFF
)




