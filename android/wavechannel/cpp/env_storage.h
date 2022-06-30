/*
* author: ChenFawang
* 2021 - 1 - 16
*/
#ifndef __ENV_STORAGE_H__
#define __ENV_STORAGE_H__
#include <jni.h>
class EnvStorage {
public:
    static int create(JavaVM *jvm);
    static void destroy();
    static int getEnv(JNIEnv** env_out);
};
#endif