/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __WT_PROTO_PHYSICAL_LAYER_H__
#define __WT_PROTO_PHYSICAL_LAYER_H__
#include "wt_proto_common.h"





typedef struct {
  void          *buff_;
  int           buff_len_;
  int           sample_rate_;
  int           sample_bit_;
}WTSendPcmBuffType;

typedef struct {
  int         bit_num_;
  double      left_phase_[COMPARE_FREQ_BIT+COMPARE_FREQ_PARITY_BIT];
  double      right_phase_[COMPARE_FREQ_BIT+COMPARE_FREQ_PARITY_BIT];
}RefPhaseInfo;

int WTPhyAnalysisNumToRealNum(int ana_num);

int WTPhysicalPcmDecode(const RecvAudioType *pcm_buf, int pcm_len, WTFreqCodeType *code);

int WTPhysicalPcmEncode(WTFreqCodeType code, void *pcm_buf, int pcm_len, RefPhaseInfo *ref_phase, int sample_bit, int sample_rate);






#endif
