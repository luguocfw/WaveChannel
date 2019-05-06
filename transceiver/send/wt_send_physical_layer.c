/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include "wt_send_physical_layer.h"
#include "wt_send_link_layer.h"
#include "proto_utils/wt_proto_common.h"
#include "proto_utils/wt_proto_physical_layer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int         sample_rate_;
  int         sample_bit_;
  WTSendPcmBuffType       pcm_info_;
}WTPhySendHanderData;

static int WTGetPcmSize(int mark_num,int sample_rate)
{
  return time_ms_to_length(mark_num*ONE_FREQ_TIME_MS, sample_rate);
}

WTSendPhyHander * WTSendPhyLayerCreateHander(WTSendPhyHanderAttr * attr)
{
  WTPhySendHanderData *hander_data = (WTPhySendHanderData *)malloc(sizeof(WTPhySendHanderData));
  if (hander_data == NULL) {
    return NULL;
  }
  WTSendPhyHander *hander = (WTSendPhyHander *)malloc(sizeof(WTSendPhyHander));
  if (hander == NULL) {
    free(hander_data);
    return NULL;
  }
  hander_data->pcm_info_.buff_ = NULL;
  hander_data->pcm_info_.buff_len_ = 0;
  hander_data->sample_bit_ = attr->sample_bit_;
  hander_data->sample_rate_ = attr->sample_rate_;
  hander->data_ = hander_data;
  return hander;
}

void WTSendPhyLayerDestroyHander(WTSendPhyHander * hander)
{
  WTPhySendHanderData *hander_data = (WTPhySendHanderData *)hander->data_;
  if (hander_data->pcm_info_.buff_ != NULL) {
    free(hander_data->pcm_info_.buff_);
    hander_data->pcm_info_.buff_ = NULL;
  }
  free(hander->data_);
  free(hander);
}

WTSendPcmBuffType * WTSendPhyLayerGetPcm(WTSendPhyHander * hander, WTSendLinkPackageS * packages)
{
  int one_package_size = COMPARE_FREQ_DATA_BIT;
  int marks_num = one_package_size * packages->package_num_;
  WTPhySendHanderData *hander_data = (WTPhySendHanderData *)hander->data_;
  hander_data->pcm_info_.buff_ = malloc((hander_data->sample_bit_ / 8)*WTGetPcmSize(marks_num, hander_data->sample_rate_));
  if (hander_data->pcm_info_.buff_ == NULL) {
    return NULL;
  }
  WTFreqCodeType temp[COMPARE_FREQ_DATA_BIT];
  int temp_len = COMPARE_FREQ_DATA_BIT;
  RefPhaseInfo ref_phase;
  ref_phase.bit_num_ = COMPARE_FREQ_BIT;
  memset(&ref_phase.left_phase_, 0, sizeof(double)*COMPARE_FREQ_BIT);
  memset(&ref_phase.right_phase_, 0, sizeof(double)*COMPARE_FREQ_BIT);
  int pcm_w_addr = 0;
  int i, j;
  int one_freq_pcm_size = WTGetPcmSize(1, hander_data->sample_rate_)*(hander_data->sample_bit_ / 8);
  for (i = 0; i < packages->package_num_; i++) {
    WTLinkFreqCodeReadFromPhyPackage(&packages->package_[i], temp, temp_len);
    for (j = temp_len-1; j >= 0; j--) {
      WTPhysicalPcmEncode(temp[j], (unsigned char *)hander_data->pcm_info_.buff_ + pcm_w_addr,
        one_freq_pcm_size, &ref_phase, hander_data->sample_bit_, hander_data->sample_rate_);
      pcm_w_addr += one_freq_pcm_size;
    }
  }
  hander_data->pcm_info_.buff_len_ = (hander_data->sample_bit_ / 8)*WTGetPcmSize(marks_num, hander_data->sample_rate_);
  hander_data->pcm_info_.sample_bit_ = hander_data->sample_bit_;
  hander_data->pcm_info_.sample_rate_ = hander_data->sample_rate_;
  return &hander_data->pcm_info_;
}

void WTSendPhyLayerReleasePcm(WTSendPhyHander * hander)
{
  WTPhySendHanderData *hander_data = (WTPhySendHanderData *)hander->data_;
  free(hander_data->pcm_info_.buff_);
  hander_data->pcm_info_.buff_ = NULL;
}
