/*
* author: ChenFawang
* 2021 - 10 - 2
*/
#ifndef __JAVA_STRING_BUFFER_H__
#define __JAVA_STRING_BUFFER_H__
#include "env_storage.h"
#include "localref_auto_release.h"
#include "jni_utils.h"
#include <string>
class JavaStringBuffer {
public:
    JavaStringBuffer(jobject obj);
    ~JavaStringBuffer();
public:
    static int initJni(JNIEnv *env);
    static void unInitJni(JNIEnv *env);
public:
    int length();
    void deleteStr(int start,int end);
    void append(const std::string &str);
private:
    jobject m_object;
    static jmethodID m_mid_length;
    static jmethodID m_mid_delete;
    static jmethodID m_mid_append;
};
#endif