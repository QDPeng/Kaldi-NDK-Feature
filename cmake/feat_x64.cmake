cmake_minimum_required(VERSION 2.8)
project(KALDIDFEATURE)
set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)
add_definitions(-DHAVE_OPENBLAS -DKALDI_DOUBLEPRECISION=0 -DHAVE_EXECINFO_H=1 -DHAVE_CXXABI_H)
set(CXX_FLAGS "-std=c++11 -Wall \
    -Wno-sign-compare -Wno-unused-local-typedefs \
    -Wno-deprecated-declarations -Winit-self \
    -m64 -msse -msse2 -pthread"
)
set(CMAKE_CXX_FLAGS_DEBUG "${CXX_FLAGS} -g2 -ggdb -O0") 
set(CMAKE_CXX_FLAGS_RELEASE "${CXX_FLAGS} -O3")
set(CMAKE_BUILD_TYPE "Release")

set(SRC_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX "${SRC_ROOT_PATH}/build/release")
message("src_root_path:${SRC_ROOT_PATH}")
# 定义要编译的文件列表变量
file(GLOB KALDI_SRC
    ${SRC_ROOT_PATH}/src/kaldi/*/*.cc
)
 
# include 头文件目录
include_directories(
    ${SRC_ROOT_PATH}/src/kaldi
    ${SRC_ROOT_PATH}/src
    ${SRC_ROOT_PATH}/thirdparty/OpenBLAS/install_x64/include
) 
    
set(TARGET_NAME compute_feats)
# 编译成可执行文件，如果编译动态库可以用：target_link_libraries
add_executable(${TARGET_NAME}
    ${SRC_ROOT_PATH}/src/main.cc
    ${SRC_ROOT_PATH}/src/feat-computer.cc
    ${KALDI_SRC}
)

target_link_libraries(${TARGET_NAME}
    ${SRC_ROOT_PATH}/thirdparty/OpenBLAS/install_x64/lib/libopenblas.a
    -lgfortran
    -lm
    -ldl
)

install(TARGETS ${TARGET_NAME} 
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    #LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    #ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)
