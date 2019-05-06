/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <crtdbg.h>
#endif

#include "proto_utils/wt_proto_common.h"
#include "interface/wave_trans_recv.h"





int main()
{
  FILE *fp = NULL;
  int ret;
  RecvAudioType pcm_buf[320];
  char data_temp[10] = { 0 };
  if (WaveTransRecvInit() != 0) {
    printf("wave trans recv init failed\n");
    return 1;
  }
  fp = fopen("test.pcm", "rb");
  if (fp == NULL) {
    printf("open file test.pcm failed\n");
    WaveTransRecvExit();
    return 1;
  }
  while (1) {
    ret = (int)fread(pcm_buf, (size_t)1, (size_t)(sizeof(RecvAudioType) * 320), fp);
    if (ret <= 0) {
      break;
    }
    WaveTransRecvSetPcm(pcm_buf, ret / sizeof(RecvAudioType));
    ret = WaveTransRecvGetContext(data_temp, sizeof(char) * 10);
    if (ret != 0) {
      data_temp[ret] = '\0';
      printf("%s", data_temp);
    }
  }
  fclose(fp);
  WaveTransRecvExit();
#ifdef WIN32
  _CrtDumpMemoryLeaks();
#endif
  return 0;
}