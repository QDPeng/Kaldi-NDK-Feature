#!/bin/bash

stage=$1

if [ $stage -eq 1 ];then
    git clone https://github.com/simonlynen/android_libs.git
fi

NDK_DIR=~/thirdparty/android-ndk-r16b
export PATH=$NDK_DIR:$PATH
if [ $stage -eq 2 ];then
    cd android_libs/lapack

    # remove some compile instructions related to tests
    sed -i 's/LOCAL_MODULE:= testlapack/#LOCAL_MODULE:= testlapack/g' jni/Android.mk
    sed -i 's/LOCAL_SRC_FILES:= testclapack.cpp/#LOCAL_SRC_FILES:= testclapack.cpp/g' jni/Android.mk
    sed -i 's/LOCAL_STATIC_LIBRARIES := lapack/#LOCAL_STATIC_LIBRARIES := lapack/g' jni/Android.mk
    sed -i 's/include $(BUILD_SHARED_LIBRARY)/#include $(BUILD_SHARED_LIBRARY)/g' jni/Android.mk

    # build for android
    ndk-build
fi
