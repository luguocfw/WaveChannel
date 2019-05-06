/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __PCM_TO_WAV_H__
#define __PCM_TO_WAV_H__
#include "proto_utils/wt_proto_physical_layer.h"





int PcmToWavGetWavSize(const WTSendPcmBuffType *pcm_buff);

int PcmToWavGetWavData(const WTSendPcmBuffType *pcm_buff, void *wav_buf, int wav_buf_len);



#endif