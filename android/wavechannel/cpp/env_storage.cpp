/*
* author: ChenFawang
* 2021 - 1 - 16
*/
#include "env_storage.h"
#include <pthread.h>
JavaVM *g_jvm = nullptr;
static pthread_key_t g_thread_key;
static pthread_once_t g_key_once = PTHREAD_ONCE_INIT;
static void JNI_ThreadDestroyed(void* value) {
    JNIEnv* env = (JNIEnv*)value;
    if (env != nullptr) {
        g_jvm->DetachCurrentThread();
        pthread_setspecific(g_thread_key, nullptr);
    }
}
static void make_thread_key() { pthread_key_create(&g_thread_key, JNI_ThreadDestroyed); }
int EnvStorage::create(JavaVM *jvm) {
    if(jvm == nullptr){
        return -1;
    }
    g_jvm = jvm;
    return 0;
}
void EnvStorage::destroy() {
    g_jvm = nullptr;
}
int EnvStorage::getEnv(JNIEnv **env_out) {
    if (g_jvm == nullptr) {
        return -1;
    }
    pthread_once(&g_key_once, make_thread_key);
    JNIEnv* env = (JNIEnv*)pthread_getspecific(g_thread_key);
    if (env) {
        *env_out = env;
        return 0;
    }
    if (g_jvm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        pthread_setspecific(g_thread_key, env);
        *env_out = env;
        return 0;
    }
    return -2;
}
