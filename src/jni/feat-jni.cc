// android jni
#include <jni.h>
#include <android/log.h>
#include "feat-computer.h"

#define TAG "asr"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

/* 定义Jni的包名 */
#define PACKAGE_NAME_FUNCTION com_asr_feat_ComputeFeat
/* 定义宏定义中用到的临时宏 */
#define JNI_FUNCTION_TEMP_2(CLASS, FUNC) Java_##CLASS##_##FUNC
#define JNI_FUNCTION_TEMP_1(CLASS, FUNC) JNI_FUNCTION_TEMP_2(CLASS, FUNC)
/* 真正用到的宏定义 */
#define JNI_FUNCTION(FUNCTION_NAME) JNI_FUNCTION_TEMP_1(PACKAGE_NAME_FUNCTION, FUNCTION_NAME)

#ifdef __cplusplus
extern "C"
{
#endif
    /*jstring转c++ string*/
    std::string jstring2str(JNIEnv *env, jstring jstr)
    {
        char *rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("GB2312");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0)
        {
            rtn = (char *)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        std::string stemp(rtn);
        free(rtn);
        return stemp;
    }

    // 初始化方法，获取对象指针
    JNIEXPORT jlong JNICALL JNI_FUNCTION(init)(JNIEnv *env, jobject obj)
    {
        FeatComputer *ptr = new FeatComputer();
        jlong handle = (jlong)ptr;
        LOGI("jni_init_handle:%ld", (long)handle);
        return handle;
    }
    //根据输入的wav格式音频文件计算特征
    JNIEXPORT jfloatArray JNICALL JNI_FUNCTION(computefile)(JNIEnv *env, jobject obj, jlong handle, jstring wavfile)
    {
        FeatComputer *ptr = reinterpret_cast<FeatComputer *>(handle);
        if (!ptr)
        {
            LOGI("jni_compute--> ptr is nullptr");
            return NULL;
        }
        std::string wf = jstring2str(env, wavfile);
        kaldi::Matrix<kaldi::BaseFloat> features;
        //计算音频文件的fbank特征
        ptr->compute_fbank(wf, &features);
        jint rows = static_cast<jint>(features.NumRows());
        jint cols = static_cast<jint>(features.NumCols());
        jint nums = rows * cols;
        jfloatArray ret = env->NewFloatArray(nums);
        LOGI("rows:%d, cols:%d", (int)rows, (int)cols);
        for (jint i = 0; i < rows; i++)
        {
            kaldi::BaseFloat *vdata = features.Row(static_cast<kaldi::int32>(i)).Data();
            jint idx = i * cols;
            //设置矩阵的每一行数据到创建的ret数组中
            env->SetFloatArrayRegion(ret, idx, cols, static_cast<jfloat *>(vdata));
        }
        return ret;
    }
    //根据输入的float数组计算fbank特征
    JNIEXPORT jfloatArray JNICALL JNI_FUNCTION(compute)(JNIEnv *env, jobject obj,
                                                        jlong handle, jfloatArray signals, jint size)
    {
        FeatComputer *ptr = reinterpret_cast<FeatComputer *>(handle);
        if (!ptr)
        {
            LOGI("jni_compute--> ptr is nullptr");
            return NULL;
        }
        kaldi::MatrixIndexT len = static_cast<kaldi::MatrixIndexT>(size);
        kaldi::BaseFloat data[len];
        //将signals数组中的数据复制到data数组中
        env->GetFloatArrayRegion(signals, 0, size, data);
       
        kaldi::Matrix<kaldi::BaseFloat> features;
        kaldi::SubVector<kaldi::BaseFloat> vecdata(data, len);
        ptr->compute_signals(vecdata, &features, 16000);
        jint rows = static_cast<jint>(features.NumRows());
        jint cols = static_cast<jint>(features.NumCols());
        jint nums = rows * cols;
        //创建需要返回的float数组，长度是nums
        jfloatArray ret = env->NewFloatArray(nums);
        LOGI("rows:%d, cols:%d", (int)rows, (int)cols);
        for (jint i = 0; i < rows; i++)
        {
            kaldi::BaseFloat *vdata = features.Row(static_cast<kaldi::int32>(i)).Data();
            jint idx = i * cols;
            env->SetFloatArrayRegion(ret, idx, cols, static_cast<jfloat *>(vdata));
        }
        return ret;
    }
   
    // 释放内存
    JNIEXPORT void JNICALL JNI_FUNCTION(destroy)(JNIEnv *env, jobject obj, jlong handle)
    {
        FeatComputer *ptr = reinterpret_cast<FeatComputer *>(handle);
        if (ptr)
        {
            delete ptr;
            LOGI("jni_destroy...");
        }
    }

#ifdef __cplusplus
}
#endif
