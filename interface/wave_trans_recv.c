/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include "wave_trans_recv.h"
#include "transceiver/recv/wt_recv_link_layer.h"
#include "transceiver/recv/wt_recv_physical_layer.h"

#ifdef __cplusplus
extern "C" {
#endif

int WaveTransRecvInit(void)
{
  if (WTRecvLinkLayerInit() != 0) {
    return -1;
  }
  if (WTRecvPhyLayerInit() != 0) {
    WTRecvLinkLayerExit();
    return -1;
  }
  return 0;
}

void WaveTransRecvExit(void)
{
  WTRecvPhyLayerExit();
  WTRecvLinkLayerExit();
}

void WaveTransRecvSetPcm(const RecvAudioType * pcm, int pcm_len)
{
  WTRecvPhyLayerSendPcm(pcm,pcm_len);
}

int WaveTransRecvGetContext(void * context, int context_len)
{
  return WTRecvLinkLayerGetData(context, context_len);
}


#ifdef __cplusplus
}
#endif