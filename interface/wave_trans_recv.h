/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __WAVE_TRANS_RECV_H__
#define __WAVE_TRANS_RECV_H__
#include "proto_utils/wt_proto_common.h"

#ifdef __cplusplus
extern "C"{
#endif

int WaveTransRecvInit(void);

void WaveTransRecvExit(void);

void WaveTransRecvSetPcm(const RecvAudioType *pcm, int pcm_len);

int WaveTransRecvGetContext(void *context, int context_len);

#ifdef __cplusplus
}
#endif
#endif
