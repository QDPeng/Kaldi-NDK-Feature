project(KALDIDFEATURE)
cmake_minimum_required(VERSION 3.6)
set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 28)

# 配置使用 NDK Standalone Toolchain 编译
set(NDK_STANDALONE_TOOLCHAIN /home/liushunpeng/thirdparty/android-ndk-r16b/mchain_x86_api23_gnustl)
#set(NDK_STANDALONE_TOOLCHAIN /home/liushunpeng/thirdparty/android-ndk-r20b/toolchain_arm)
set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 23)
set(CMAKE_C_COMPILER ${NDK_STANDALONE_TOOLCHAIN}/bin/i686-linux-android-clang)
set(CMAKE_CXX_COMPILER ${NDK_STANDALONE_TOOLCHAIN}/bin/i686-linux-android-clang++)
set(CMAKE_LINKER ${NDK_STANDALONE_TOOLCHAIN}/bin/i686-linux-android-ld)
set(CMAKE_FIND_ROOT_PATH ${NDK_STANDALONE_TOOLCHAIN})
set(CMAKE_ANDROID_ARCH_ABI x86)

# 使用 NDK 提供的头文件
add_definitions("--sysroot=${NDK_STANDALONE_TOOLCHAIN}/sysroot")

message(${CMAKE_CXX_COMPILER})
#add_definitions(-DHAVE_OPENBLAS -DKALDI_DOUBLEPRECISION=0 -DHAVE_EXECINFO_H=0 -DHAVE_CXXABI_H)
add_definitions(-DHAVE_OPENBLAS -DKALDI_DOUBLEPRECISION=0)
#set(CXX_FLAGS "-std=c++11 -Wall \
#    -Wno-sign-compare -Wno-unused-local-typedefs \
#    -Wno-deprecated-declarations -Winit-self \
#    -m64 -msse -msse2 -pthread"
#)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS} -std=c++11 -fPIE -g2 -ggdb -O0") 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS} -std=c++11 -fPIE -O3")
set(CMAKE_BUILD_TYPE "Release")

set(SRC_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX "${SRC_ROOT_PATH}/build/release/x86")
message("src_root_path:${SRC_ROOT_PATH}")
# 定义要编译的文件列表变量
file(GLOB KALDI_SRC
    ${SRC_ROOT_PATH}/src/kaldi/*/*.cc
)
 
set(OPENBLAS_LIB ${SRC_ROOT_PATH}/thirdparty/OpenBLAS/install_armx86_r16b)
set(LAPACK_LIB ${SRC_ROOT_PATH}/thirdparty/android_libs/lapack/obj/local/x86)
# include 头文件目录
include_directories(
    ${SRC_ROOT_PATH}/src/kaldi
    ${SRC_ROOT_PATH}/src
    ${OPENBLAS_LIB}/include
) 
    
set(TARGET_NAME compute_feats)
# 编译成可执行文件，如果编译动态库可以用：target_link_libraries
add_library(${TARGET_NAME} SHARED
    ${SRC_ROOT_PATH}/src/jni/feat-jni.cc
    ${SRC_ROOT_PATH}/src/feat-computer.cc
    ${KALDI_SRC}
)

target_link_libraries(${TARGET_NAME}
    -lm
    log
    ${OPENBLAS_LIB}/lib/libopenblas.a
    ${LAPACK_LIB}/libclapack.a
    ${LAPACK_LIB}/libf2c.a
    ${LAPACK_LIB}/liblapack.a
    ${LAPACK_LIB}/libblas.a
)

install(TARGETS ${TARGET_NAME} 
    # RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    #ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)
