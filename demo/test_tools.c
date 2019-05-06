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
#ifdef WIN32
#include <crtdbg.h>
#endif

#include "interface/wave_trans_send.h"

#define DATA_FILE           "test.txt"
#define AUDIO_FILE          "test.wav"

int GetFileSize(char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return -1;
  }
  fseek(fp, 0L, SEEK_END);
  int size = ftell(fp);
  fclose(fp);
  return size;
}


int main()
{
  //_CrtSetBreakAlloc(80);
  FILE *fp = NULL;
  FILE *wav_fp = NULL;
  unsigned char *data_buf = NULL;
  WaveTransSendHander *hander = NULL;
  int buf_len = GetFileSize(DATA_FILE) + 1;
  data_buf = (unsigned char *)malloc(sizeof(unsigned char)*buf_len);
  if (data_buf == NULL) {
    goto err_exit;
  }
  fp = fopen(DATA_FILE, "r");
  if (fp == NULL) {
    goto err_exit;
  }
  int ret;
  ret = fread(data_buf, 1, sizeof(unsigned char)*buf_len, fp);
  if (ret <= 0) {
    goto err_exit;
  }
  fclose(fp);
  fp = NULL;
  data_buf[ret] = '\n';
  WaveTransSendAttr attr;
  attr.sample_bit_ = 16;
  attr.sample_rate_ = 16000;
  hander = WaveTransSendCreateHander(&attr);
  if (hander == NULL) {
    goto err_exit;
  }
  WaveTransWavInfo *wav_info;
  wav_info = WaveTransSendGetWav(hander, data_buf, buf_len);
  if (wav_info == NULL) {
    goto err_exit;
  }
  wav_fp = fopen(AUDIO_FILE, "wb");
  if (wav_fp == NULL) {
    goto err_exit;
  }
  ret = fwrite(wav_info->wav_buff_, 1, wav_info->buff_len_, wav_fp);
  if (ret != wav_info->buff_len_) {
    goto err_exit;
  }
  fclose(wav_fp);
  wav_fp = NULL;
  free(data_buf);
  WaveTransSendDestroyHander(hander);
#ifdef WIN32
  _CrtDumpMemoryLeaks();
#endif
  return 0;
err_exit:
  if (wav_fp != NULL) {
    fclose(wav_fp);
  }
  if (fp != NULL) {
    fclose(fp);
  }
  if (hander != NULL) {
    WaveTransSendDestroyHander(hander);
  }
  if (data_buf != NULL) {
    free(data_buf);
  }
#ifdef WIN32
  _CrtDumpMemoryLeaks();
#endif
  return 1;
}