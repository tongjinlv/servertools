project( APP )
cmake_minimum_required( VERSION 2.6 )
add_definitions(-std=c++14)





file( GLOB SRCS *.c *.cpp *.cc *.h *.hpp )
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_executable( ${PROJECT_NAME} ${SRCS} )
include_directories("spdlog/include")



include(ExternalProject)
set(SPDLOG_SRC_PATH "spdlog")
ExternalProject_Add(spdlog
    SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/spdlog
    DOWNLOAD_DIR  ${CMAKE_HOME_DIRECTORY}/dl
    DOWNLOAD_NAME spdlog-1.4.2.tar.gz
    URL https://github.com/gabime/spdlog/archive/v1.4.2.tar.gz
    URL_HASH SHA1=bad3caf7e4e1c2f299ccaaa8f56ff7edc0f36530
    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DTOOLS_DIRECTORY=${TOOLS_DIRECTORY}
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}
        -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        -DSPDLOG_BUILD_TESTING=OFF
)








