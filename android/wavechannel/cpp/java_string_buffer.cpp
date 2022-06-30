/*
* author: ChenFawang
* 2021 - 10 - 2
*/
#include "java_string_buffer.h"
jmethodID JavaStringBuffer::m_mid_length = nullptr;
jmethodID JavaStringBuffer::m_mid_delete = nullptr;
jmethodID JavaStringBuffer::m_mid_append = nullptr;
JavaStringBuffer::JavaStringBuffer(jobject obj) {
    JNIEnv *env = nullptr;
    assert(EnvStorage::getEnv(&env) == 0);
    m_object = env->NewGlobalRef(obj);
}
JavaStringBuffer::~JavaStringBuffer() {
    JNIEnv *env = nullptr;
    assert(EnvStorage::getEnv(&env) == 0);
    env->DeleteGlobalRef(m_object);
}
int JavaStringBuffer::initJni(JNIEnv *env) {
    jclass cls = env->FindClass("java/lang/StringBuffer");
    assert(!ExceptionCheck(env));
    m_mid_length = env->GetMethodID(cls,"length", "()I");
    assert(!ExceptionCheck(env));
    m_mid_delete = env->GetMethodID(cls,"delete", "(II)Ljava/lang/StringBuffer;");
    assert(!ExceptionCheck(env));
    m_mid_append = env->GetMethodID(cls,"append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
    assert(!ExceptionCheck(env));
    return 0;
}
void JavaStringBuffer::unInitJni(JNIEnv *env) {

}
int JavaStringBuffer::length() {
    JNIEnv *env = nullptr;
    assert(EnvStorage::getEnv(&env) == 0);
    int len = env->CallIntMethod(m_object,m_mid_length);
    assert(!ExceptionCheck(env));
    return len;
}
void JavaStringBuffer::deleteStr(int start, int end) {
    JNIEnv *env = nullptr;
    assert(EnvStorage::getEnv(&env) == 0);
    env->CallObjectMethod(m_object,m_mid_delete,start,end);
    assert(!ExceptionCheck(env));
}
void JavaStringBuffer::append(const std::string &str) {
    JNIEnv *env = nullptr;
    assert(EnvStorage::getEnv(&env) == 0);
    jstring jstr = env->NewStringUTF(str.c_str());
    LocalObjectAutoRelease auto_release(jstr,env);
    env->CallObjectMethod(m_object,m_mid_append,jstr);
    assert(!ExceptionCheck(env));
}