/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __WT_SEND_LINK_LAYER_H__
#define __WT_SEND_LINK_LAYER_H__
#include "proto_utils/wt_proto_link_layer.h"


typedef struct {
  void          *data_;
}WTSendLinkHander;

typedef struct {
  int                           package_num_;
  WaveTransPhyPackage    *package_;
}WTSendLinkPackageS;



WTSendLinkHander * WTSendLinkLayerCreateHander(void);

void WTSendLinkLayerDestroyHander(WTSendLinkHander *hander);

WTSendLinkPackageS *WTSendLinkLayerGetPackage(WTSendLinkHander *hander, const void *context, int context_len);

void WTSendLinkLayerReleasePackage(WTSendLinkHander *hander);

#endif
