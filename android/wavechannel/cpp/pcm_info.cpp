/*
* author: ChenFawang
* 2022 - 5 - 8
*/

#include "pcm_info.h"
#include "env_storage.h"

jfieldID JavaPcmInfo::m_fid_buf = nullptr;
jfieldID JavaPcmInfo::m_fid_rate = nullptr;
jfieldID JavaPcmInfo::m_fid_bits = nullptr;
int JavaPcmInfo::initJni(JNIEnv *env) {
    jclass cls = env->FindClass("com/cfwang/wavechannel/Native$PcmInfo");
    m_fid_buf = env->GetFieldID(cls,"pcmBuf", "[B");
    m_fid_bits = env->GetFieldID(cls,"bits", "I");
    m_fid_rate = env->GetFieldID(cls,"rate", "I");
    return 0;
}
void JavaPcmInfo::unInitJni(JNIEnv *env) {
}
int JavaPcmInfo::setToObject(const WaveTransPcmInfo &info, jobject obj) {
    JNIEnv *env = nullptr;
    if(EnvStorage::getEnv(&env) != 0){
        return -1;
    }
    if(info.pcm_buff_ != nullptr && info.buff_len_ > 0){
        jbyteArray array = env->NewByteArray(info.buff_len_);
        env->SetByteArrayRegion(array,0,info.buff_len_,(jbyte *)info.pcm_buff_);
        env->SetObjectField(obj,m_fid_buf,array);
    }else{
        env->SetObjectField(obj,m_fid_buf,(jobject)nullptr);
    }
    env->SetIntField(obj,m_fid_bits,info.sample_bit_);
    env->SetIntField(obj,m_fid_rate,info.sample_rate_);
    return 0;
}

