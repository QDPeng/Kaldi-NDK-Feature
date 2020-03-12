### 基于kaldi实现fbank特征提取
支持arm平台和x86平台，安卓动态库需要在linux平台上进行编译

#### 下载android-ndk-r16

```shell
#创建目录
mkdir ~/thirdparty
cd ~/thirdparty
#下载android-ndk-r16b-linux-x86_64.zip
wget https://dl.google.com/android/repository/android-ndk-r16b-linux-x86_64.zip?hl=zh_cn
#解压
unzip android-ndk-r16b-linux-x86_64.zip
#创建独立工具链
cd android-ndk-r16
# 创建armeabi-v7工具链
python build/tools/make_standalone_toolchain.py --arch arm --api 23 --stl gnustl --package-dir mchain_arm_api23_gnustl
# 创建x86工具链
python build/tools/make_standalone_toolchain.py --arch x86 --api 23 --stl gnustl --package-dir mchain_x86_api23_gnustl
```

#### 编译openblas
```shell
cd thirdparty/
make -f openblas.mk download
# 编译armeabi-v7a版本
sh openblas_arm32v7.sh
# 编译x86版本
sh openblas_armx86.sh
```

#### 编译lapack

```shell
# 下载lapack 安卓库
sh androidlibs.sh 1
# 编译armeabi-v7a和x86版本
sh androidlibs.sh 2
```

#### 编译安卓动态库

修改CMakeLists.txt文件

```cmake
cmake_minimum_required(VERSION 3.6)
#include(cmake/feat_armx86.cmake)
include(cmake/feat_armv7a.cmake)
```

```shell
# 创建build目录
mkdir build
cd build
cmake ..
# 执行make -j 生成libcompute_feats.so 这个是armeabi-v7a版本的
make -j
```

同样注释掉CMakeLists.txt文件中的armv7a版本，打开armx86版本就可以编译x86版本。

#### JNI函数和Java类型

jni中定义的包名、类名要和java创建的类名和包名要统一

jni相关函数：

```c++
/* 定义Jni的包名 */
#define PACKAGE_NAME_FUNCTION com_asr_feat_ComputeFeat
// 初始化方法，获取对象指针
JNIEXPORT jlong JNICALL JNI_FUNCTION(init)(JNIEnv *env, jobject obj);
    
//根据输入的float数组计算fbank特征
JNIEXPORT jfloatArray JNICALL JNI_FUNCTION(compute)(JNIEnv *env, jobject obj,
                                                        jlong handle, jfloatArray signals, jint size);
// 释放内存                                                  
JNIEXPORT void JNICALL JNI_FUNCTION(destroy)(JNIEnv *env, jobject obj, jlong handle)                                    
```

java对应的包名和类名，

```java
package com.asr.feat;

public class ComputeFeat {
    static {
        //加载动态连接库
        System.loadLibrary("compute_feats");
    }
    //c++对象句柄
    private long handle;

    public ComputeFeat() {
        //c++程序初始化，返回对象句柄
        handle = init();
    }
    //计算音频特征
    public float[] compute(float[] signals, int size) {
        return compute(handle, signals, size);
    }

    public float[] compute(String wavfile) {
        return computefile(handle, wavfile);
    }
    //释放内存资源
    public void destroy() {
        destroy(handle);
    }
    //jni对应的函数
    private native long init();

    private native void destroy(long handle);

    private native float[] compute(long handle, float[] signals, int size);

}

```

#### 特别感谢

- https://github.com/kaldi-asr/kaldi
- https://github.com/simonlynen/android_libs
- https://developer.android.google.cn/ndk/guides/standalone_toolchain