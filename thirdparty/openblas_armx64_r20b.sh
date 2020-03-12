#!/bin/bash
#将ndk目录加入环境变量
export NDK_DIR=~/thirdparty/android-ndk-r20b
#将toolchain目录加入环境变量
export MY_TOOLCHAIN=${NDK_DIR}/toolchain_arm
#生成独立工具链
#$NDK__DIR/build/tools/make_standalone_toolchain.py --arch arm --api 16 --install-dir ${MY_TOOLCHAIN}
#将独立工具链工具加入环境变量(clang和arm-linux-androideabi-*)
export PATH=${MY_TOOLCHAIN}/bin:$PATH
#配置要使用的工具
export target_host=x86_64-linux-android
export AR=${target_host}-ar
export AS=${target_host}-as
export CC=${target_host}28-clang
export CXX=${target_host}28-clang++
export LD=${target_host}-ld
export STRIP=${target_host}-strip

#设置LDFLAGS，以便链接器找到合适的libgcc
export LDFLAGS="-L${MY_TOOLCHAIN}/lib/gcc/arm-linux-androideabi/4.9.x -lm"
#设置clang交叉编译标志
export CLANG_FLAGS="-target ${target_host} --sysroot ${MY_TOOLCHAIN}/sysroot -gcc-toolchain ${MY_TOOLCHAIN}/${target_host}"
#进入OpenBLAS源码目录
cd OpenBLAS
make clean
prefix=`pwd`/install_armx64_r20b
rm -rf $prefix
#编译ARMV7环境静态库
make TARGET=x86_64 ONLY_CBLAS=1 AR=$AR CC="clang ${CLANG_FLAGS}" HOSTCC=gcc ARM_SOFTFP_ABI=1 NO_SHARED=1 -j4
#安装静态库
make NO_SHARED=1 PREFIX=$prefix install
