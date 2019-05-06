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
#include "proto_utils/wt_proto_physical_layer.h"



#if 1
int main()
{
  short pcm_buf[128];
  memset(pcm_buf, 0, sizeof(short) * 128);
  int buf_len = 128;
  FILE *fp = NULL;
  fp = fopen("test.pcm", "rb");
  if (fp == NULL) {
    printf("open file phy.pcm failed\n");
    return 1;
  }
  WTFreqCodeType mark;
  int ret;
  while ((ret = fread(pcm_buf, 1, sizeof(short)*buf_len, fp)) == buf_len * sizeof(short)) {
    if (WTPhysicalPcmDecode(pcm_buf, buf_len, &mark) != 0) {
      continue;
    }
    printf(" %d ", mark);
  }
  fclose(fp);
  int i;
  return 0;
}
#endif

#if 0
int main()
{
  short pcm_buf[384];
  memset(pcm_buf, 0, sizeof(short) * 384);
  int buf_len = 384;
  FILE *fp = NULL;
  fp = fopen("phy.pcm", "wb");
  if (fp == NULL) {
    printf("open file phy.pcm failed\n");
    return 1;
  }
  WTFreqCodeType mark;
  RefPhaseInfo ref_phase;
  memset(&ref_phase, 0, sizeof(RefPhaseInfo));
  int i;
  for (i = 0; i < 512; i++) {
    mark = i;
   // mark = 2;
    if (WTPhysicalPcmEncode(mark, pcm_buf, sizeof(short)*buf_len, &ref_phase, 16, 16000) != 0) {
      fclose(fp);
      return 1;
    }
    int ret = fwrite(pcm_buf, 1, sizeof(short)*buf_len, fp);
    if (ret != sizeof(short)*buf_len) {
      printf("write file failed\n");
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}
#endif