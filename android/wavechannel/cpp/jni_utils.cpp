/*
* author: ChenFawang
* 2021 - 1 - 16
*/
#include "jni_utils.h"
bool ExceptionCheck(JNIEnv* env){
    if(!env->ExceptionCheck())
    {
        return false;
    }
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
}