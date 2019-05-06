/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __WT_RECV_PHYSICAL_LAYER_H__
#define __WT_RECV_PHYSICAL_LAYER_H__
#include "proto_utils/wt_proto_physical_layer.h"
#include "proto_utils/wt_proto_common.h"


int WTRecvPhyLayerInit();

void WTRecvPhyLayerExit();

void WTRecvPhyLayerSendPcm(const RecvAudioType *pcm, int pcm_len);

int WTRecvPhyLayerGetData(WTFreqCodeType *data_buf, int buf_len);


#endif
