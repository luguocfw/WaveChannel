/*
* author: ChenFawang
* 2022 - 5 - 8
*/
#ifndef __WAV_INFO_H__
#define __WAV_INFO_H__
#include "interface/wave_trans_send.h"
#include <jni.h>
class JavaWavInfo {
public:
    static int initJni(JNIEnv *env);
    static void unInitJni(JNIEnv *env);
    static int setToObject(const WaveTransWavInfo &info,jobject obj);
private:
    static jfieldID m_fid_buf;
};
#endif