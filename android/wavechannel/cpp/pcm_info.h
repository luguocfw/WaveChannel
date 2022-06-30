/*
* author: ChenFawang
* 2022 - 5 - 8
*/
#ifndef __PCM_INFO_H__
#define __PCM_INFO_H__
#include "interface/wave_trans_send.h"
#include <jni.h>
class JavaPcmInfo {
public:
    static int initJni(JNIEnv *env);
    static void unInitJni(JNIEnv *env);
    static int setToObject(const WaveTransPcmInfo &info,jobject obj);
private:
    static jfieldID m_fid_buf;
    static jfieldID m_fid_rate;
    static jfieldID m_fid_bits;
};
#endif