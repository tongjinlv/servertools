project( APP )
cmake_minimum_required( VERSION 2.6 )
add_definitions(-std=c++14)



include(ExternalProject)
set(SPDLOG_SRC_PATH "spdlog")
ExternalProject_Add(spdlog
    SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/lib/spdlog
    DOWNLOAD_DIR  ${CMAKE_HOME_DIRECTORY}/lib/dl
    DOWNLOAD_NAME spdlog-0.16.2.tar.gz
    URL https://github.com/gabime/spdlog/archive/v0.16.2.tar.gz
    # URL http://video.trtos.com/v0.16.2.tar.gz
    URL_HASH SHA1=4f2123e9d83dda8a94a6845c256a5858578046fc
    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DTOOLS_DIRECTORY=${TOOLS_DIRECTORY}
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}
        -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        -DSPDLOG_BUILD_TESTING=OFF
)


include_directories("spdlog/include")
file( GLOB SRCS *.c *.cpp *.cc *.h *.hpp )
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_executable( ${PROJECT_NAME} ${SRCS} )






