/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include "wt_recv_link_layer.h"
#include "proto_utils/wt_proto_link_layer.h"
#include "wt_recv_physical_layer.h"
#include "proto_utils/wt_proto_common.h"

#include <stdlib.h>
#include <string.h>

static WaveTransPhyPackage package_temp_;

static int GetNextFreqCodePackage(WaveTransLinkPackage *package)
{
  int ret;
  WTFreqCodeType code_temp;
  int loop_num = 10;
  while (loop_num--) {
    memset(&code_temp, 0, sizeof(WTFreqCodeType));
    ret = WTRecvPhyLayerGetData(&code_temp, 1);
    if (ret <= 0) {
      return -1;
    }
    WTLinkFreqCodeWriteToPhyPackage(code_temp, &package_temp_);
    if (WTLinkCheckStCode(package_temp_.st_mark_, COMPARE_FREQ_ST_NUM) != 1) {
      continue;
    }
    WTLinkPackageDecode(&package_temp_, package);
    WTLinkChecksumDecode(package);
    memset(&package_temp_, 0, sizeof(WaveTransPhyPackage));
    return 0;
  }
  return -1;
}

int WTRecvLinkLayerInit()
{
  memset(&package_temp_, 0, sizeof(WaveTransPhyPackage));
  return 0;
}

void WTRecvLinkLayerExit()
{
}

int WTRecvLinkLayerGetData(void * buf, int buf_len)
{
  WaveTransLinkPackage one_package;
  int buf_w_addr = 0;
  while (buf_len - buf_w_addr >= COMPARE_FREQ_DATA_NUM) {
    if (GetNextFreqCodePackage(&one_package) != 0) {
      break;
    }
    memcpy(((unsigned char *)buf + buf_w_addr), one_package.byte_data_, sizeof(unsigned char)*one_package.real_data_num_);
    buf_w_addr += one_package.real_data_num_;
  }
  return buf_w_addr;
}
