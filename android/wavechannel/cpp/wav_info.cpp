/*
* author: ChenFawang
* 2022 - 5 - 8
*/
#include "wav_info.h"
#include "env_storage.h"
jfieldID JavaWavInfo::m_fid_buf = nullptr;
int JavaWavInfo::initJni(JNIEnv *env) {
    jclass cls = env->FindClass("com/cfwang/wavechannel/Native$WavInfo");
    m_fid_buf = env->GetFieldID(cls,"wavBuf", "[B");
    return 0;
}
void JavaWavInfo::unInitJni(JNIEnv *env) {

}
int JavaWavInfo::setToObject(const WaveTransWavInfo &info, jobject obj) {
    JNIEnv *env = nullptr;
    if(EnvStorage::getEnv(&env) != 0){
        return -1;
    }
    if(info.wav_buff_ != nullptr && info.buff_len_ > 0){
        jbyteArray array = env->NewByteArray(info.buff_len_);
        env->SetByteArrayRegion(array,0,info.buff_len_,(jbyte *)info.wav_buff_);
        env->SetObjectField(obj,m_fid_buf,array);
    }else{
        env->SetObjectField(obj,m_fid_buf,(jobject)nullptr);
    }
    return 0;
}

