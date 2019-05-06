/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "interface/wave_trans_send.h"
#include "audio_codec/pcm_to_wav.h"

#ifdef WIN32
#include <crtdbg.h>
#endif


int main(void)
{
  int ret;
  FILE *fp = NULL;
  char test_context[] = "\n\r\n\r'Z\n'uH:\n_Z'ZZZ,H5C~$:'Z,C\n3\\J05$?55\n1516884990339\nipc\n88888888\r\n\r\n";
  WaveTransSendHander *wt_send_hander = NULL;
  WaveTransSendAttr attr;
  attr.sample_bit_ = 16;
  attr.sample_rate_ = 16000;
  wt_send_hander = WaveTransSendCreateHander(&attr);
  if (wt_send_hander == NULL) {
    printf("wave trans send create hander failed\n");
    return 1;
  }
  WaveTransPcmInfo *pcm_info = WaveTransSendGetPcm(wt_send_hander, test_context, (int)strlen(test_context));
  if (pcm_info == NULL) {
    printf("wave trans send get pcm failed\n");
    WaveTransSendDestroyHander(wt_send_hander);
    return 1;
  }
  fp = fopen("test.pcm", "wb");
  if (fp == NULL) {
    printf("open file test.pcm failed\n");
    WaveTransSendDestroyHander(wt_send_hander);
    return 1;
  }
  ret = (int)fwrite(pcm_info->pcm_buff_, (size_t)1, (size_t)pcm_info->buff_len_, fp);
  if (ret != pcm_info->buff_len_) {
    fclose(fp);
    WaveTransSendDestroyHander(wt_send_hander);
    return 1;
  }
  fclose(fp);
  WaveTransWavInfo *wav_info = WaveTransSendGetWav(wt_send_hander, test_context, strlen(test_context));
  if (wav_info == NULL) {
    printf("wave trans send get wav failed\n");
    WaveTransSendDestroyHander(wt_send_hander);
    return 1;
  }
  fp = fopen("test.wav", "wb");
  if (fp == NULL) {
    printf("open file test.pcm failed\n");
    WaveTransSendDestroyHander(wt_send_hander);
    return 1;
  }
  ret = (int)fwrite(wav_info->wav_buff_, (size_t)1, (size_t)wav_info->buff_len_, fp);
  if (ret != wav_info->buff_len_) {
    fclose(fp);
    WaveTransSendDestroyHander(wt_send_hander);
    return 1;
  }
  fclose(fp);
  WaveTransSendDestroyHander(wt_send_hander);
#ifdef WIN32
  _CrtDumpMemoryLeaks();
#endif
  return 0;
}