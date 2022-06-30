/*
* author: ChenFawang
* 2022 - 5 - 8
*/
#include <jni.h>
#include "interface/wave_trans_send.h"
#include "interface/wave_trans_recv.h"
#include "env_storage.h"
#include "pcm_info.h"
#include "wav_info.h"
#include "java_string_buffer.h"
#define TEMP_BUF_SIZE           (1024)
static JavaVM* g_jvm = nullptr;
static uint8_t *g_temp_buf[TEMP_BUF_SIZE] = {0};
static jlong waveTransSendCreateHander(JNIEnv* env, jclass thiz,jint bits,jint rate){
    WaveTransSendAttr attr;
    attr.sample_bit_ = bits;
    attr.sample_rate_ = rate;
    WaveTransSendHander *handler = WaveTransSendCreateHander(&attr);
    if(handler == nullptr){
        return 0;
    }
    return (jlong)handler;
}
static void waveTransSendDestroyHander(JNIEnv* env, jclass thiz,jlong handle){
    if(handle == 0){
        return;
    }
    WaveTransSendDestroyHander((WaveTransSendHander *)handle);
}
static jint waveTransSendGetPcm(JNIEnv* env, jclass thiz,
                                jlong handle,jbyteArray context,jobject pcmOut){
	if(handle == 0){
		return -1;
	}
	jbyte *arr = env->GetByteArrayElements(context,0);
	WaveTransPcmInfo *info = WaveTransSendGetPcm((WaveTransSendHander *)handle,
												 arr,env->GetArrayLength(context));
	if(info == nullptr){
		return -2;
	}
	if(JavaPcmInfo::setToObject(*info,pcmOut) != 0){
		return -3;
	}
	return 0;
}
static jint waveTransSendGetWav(JNIEnv* env, jclass thiz,
                                jlong handle,jbyteArray context,jobject wavOut){
	if(handle == 0){
		return -1;
	}
	jbyte *arr = env->GetByteArrayElements(context,0);
	WaveTransWavInfo *info = WaveTransSendGetWav((WaveTransSendHander *)handle,
												 arr,env->GetArrayLength(context));
	env->ReleaseByteArrayElements(context,arr,JNI_ABORT);
	if(info == nullptr){
		return -2;
	}
	if(JavaWavInfo::setToObject(*info,wavOut) != 0){
		return -3;
	}
	return 0;
}
static jint waveTransRecvInit(JNIEnv* env, jclass thiz){
	return WaveTransRecvInit();
}
static void waveTransRecvExit(JNIEnv* env, jclass thiz){
	WaveTransRecvExit();
}
static void waveTransRecvSetPcm(JNIEnv* env, jclass thiz,jbyteArray pcm){
	jbyte  *arr = env->GetByteArrayElements(pcm,0);
	WaveTransRecvSetPcm((RecvAudioType *)arr,env->GetArrayLength(pcm) / sizeof(RecvAudioType));
	env->ReleaseByteArrayElements(pcm,arr,JNI_ABORT);
}
static jint waveTransRecvGetContext(JNIEnv* env, jclass thiz,jbyteArray contextBuf){
    int buf_len = env->GetArrayLength(contextBuf);
    int read_len = buf_len < TEMP_BUF_SIZE ? buf_len : TEMP_BUF_SIZE;
    int ret = WaveTransRecvGetContext(g_temp_buf,read_len);
    if(ret > 0){
        env->SetByteArrayRegion(contextBuf,0,ret,(jbyte *)g_temp_buf);
    }
    return ret;
}
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
static JNINativeMethod sdkNativeMethods[] = {
    {"waveTransSendCreateHander", "(II)J",(void*)waveTransSendCreateHander},
    {"waveTransSendDestroyHander", "(J)V",(void*)waveTransSendDestroyHander},
    {"waveTransSendGetPcm",
     "(J[BLcom/cfwang/wavechannel/Native$PcmInfo;)I",
                        (void*)waveTransSendGetPcm},
    {"waveTransSendGetWav",
            "(J[BLcom/cfwang/wavechannel/Native$WavInfo;)I",
            (void*)waveTransSendGetWav},
    {"waveTransRecvInit", "()I",(void*)waveTransRecvInit},
    {"waveTransRecvExit", "()V",(void*)waveTransRecvExit},
    {"waveTransRecvSetPcm", "([B)V",(void*)waveTransRecvSetPcm},
    {"waveTransRecvGetContext", "([B)I",
     (void*)waveTransRecvGetContext},
};

jint JNI_OnLoad(JavaVM* pVm, void* reserved)
{
	JNIEnv* env;
	if (pVm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
	{
        return -1;
	}
    g_jvm = pVm;
	jclass cls = env->FindClass("com/cfwang/wavechannel/Native");
	env->RegisterNatives(cls, sdkNativeMethods, NELEM(sdkNativeMethods));
	EnvStorage::create(pVm);
	JavaPcmInfo::initJni(env);
	JavaWavInfo::initJni(env);
	JavaStringBuffer::initJni(env);
	return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM* pVm, void* reserved)
{
	JNIEnv* env;
	if (pVm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		return;
	}

	jclass cls = env->FindClass("com/cfwang/wavechannel/Native");
	if (cls != NULL)
	{
		env->UnregisterNatives(cls);
	}
	JavaPcmInfo::unInitJni(env);
	JavaWavInfo::unInitJni(env);
	JavaStringBuffer::unInitJni(env);
	EnvStorage::destroy();
}
