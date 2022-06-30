/*
* author: ChenFawang
* 2021 - 1 - 16
*/
#ifndef __ANDROID_JNI_LOCALREF_AUTO_RELEASE_H__
#define __ANDROID_JNI_LOCALREF_AUTO_RELEASE_H__
#include <jni.h>
class LocalObjectAutoRelease {
private:
    LocalObjectAutoRelease() {
        m_env = nullptr;
        initData();
    }
public:
    LocalObjectAutoRelease(jobject object, JNIEnv* env) : LocalObjectAutoRelease() {
        m_env = env;
        m_object = object;
    }
    LocalObjectAutoRelease(jclass j_class, JNIEnv* env) : LocalObjectAutoRelease() {
        m_env = env;
        m_class = j_class;
    }
    LocalObjectAutoRelease(jbyteArray j_array, JNIEnv* env) : LocalObjectAutoRelease() {
        m_env = env;
        m_byte_array = j_array;
    }
    LocalObjectAutoRelease(jintArray j_array, JNIEnv* env) : LocalObjectAutoRelease() {
        m_env = env;
        m_int_array = j_array;
    }
    LocalObjectAutoRelease(jlongArray j_array, JNIEnv* env) : LocalObjectAutoRelease() {
        m_env = env;
        m_long_array = j_array;
    }
    LocalObjectAutoRelease(jstring str, JNIEnv* env) : LocalObjectAutoRelease() {
        m_env = env;
        m_string = str;
    }
    ~LocalObjectAutoRelease() {
        if (m_object != nullptr) {
            m_env->DeleteLocalRef(m_object);
        } else if (m_class != nullptr) {
            m_env->DeleteLocalRef(m_class);
        }else if(m_byte_array != nullptr){
            m_env->DeleteLocalRef(m_byte_array);
        }else if(m_int_array != nullptr){
            m_env->DeleteLocalRef(m_int_array);
        }else if(m_long_array != nullptr){
            m_env->DeleteLocalRef(m_long_array);
        }else if(m_string != nullptr){
            m_env->DeleteLocalRef(m_string);
        }
    }

private:
    void initData() {
        m_object = nullptr;
        m_byte_array  = nullptr;
        m_class  = nullptr;
        m_int_array = nullptr;
        m_long_array = nullptr;
        m_string = nullptr;
    }
    jobject m_object;
    jclass m_class;
    jbyteArray m_byte_array;
    jintArray m_int_array;
    jlongArray m_long_array;
    jstring m_string;
    JNIEnv* m_env;
};
#endif