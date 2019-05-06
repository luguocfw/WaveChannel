/*
#
# Copyright 2018, sohu ipc
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: fawangchen@sohu-inc.com
*/
#include "wt_recv_physical_layer.h"
#include "buff_utils/ring_buff.h"

#include <stdio.h>
#include <string.h>
#define FREQ_ANA_BUF_SIZE               time_ms_to_length(FREQ_ANALYZE_SAMPLE_TIME_MS,RECV_SAMPLE_RATE)

#ifndef WIN32
#include <pthread.h>
static pthread_mutex_t buff_mutex_ = PTHREAD_MUTEX_INITIALIZER;
#define BUF_LOCK_INIT()               pthread_mutex_init(&buff_mutex_,NULL)
#define BUF_LOCK_EXIT()               pthread_mutex_destroy(&buff_mutex_)
#define BUF_LOCK()                    pthread_mutex_lock(&buff_mutex_)
#define BUF_UNLOCK()                  pthread_mutex_unlock(&buff_mutex_)
#else
#define BUF_LOCK_INIT()         
#define BUF_LOCK_EXIT()            
#define BUF_LOCK()                   
#define BUF_UNLOCK() 
#endif

typedef struct {
  WTFreqCodeType        code_;
  int                   code_num_;
  int                   already_num_;
}OldPhyCodeRef;

#define REF_CODE_INIT_DATA        (0xffff)


#define RING_BUFF_LEN         (2048)





static RingBuffFd     *ring_buff_fd_ = NULL;
static OldPhyCodeRef   old_code_ref_;
static WTFreqCodeType code_ref_temp_;
static RecvAudioType pcm_buf_[FREQ_ANA_BUF_SIZE];
static int pcm_buf_w_addr_ = 0;




static int GetNextFreqCode(WTFreqCodeType *data)
{
  WTFreqCodeType code;
  int ret;
  BUF_LOCK();
  while ((ret = RingBuffReadData(ring_buff_fd_, &code, sizeof(WTFreqCodeType))) != 0) {
    if (code == old_code_ref_.code_) {
      old_code_ref_.code_num_++;
      code_ref_temp_ = REF_CODE_INIT_DATA;
      int old_num = WTPhyAnalysisNumToRealNum(old_code_ref_.code_num_);
      if (old_num > old_code_ref_.already_num_) {
        //WtLog(" #%ld ", old_freq_code_ref_.freq_code_);
        *data = code;
        old_code_ref_.already_num_++;
        BUF_UNLOCK();
        return 0;
      }
    }
    else {
      if (code == code_ref_temp_) {
        old_code_ref_.code_ = code;
        old_code_ref_.code_num_ = 2;
        old_code_ref_.already_num_ = 0;
        code_ref_temp_ = REF_CODE_INIT_DATA;
        int old_num = WTPhyAnalysisNumToRealNum(old_code_ref_.code_num_);
        if (old_num > old_code_ref_.already_num_) {
          //WtLog(" #%ld ", old_freq_code_ref_.freq_code_);
          *data = code;
          old_code_ref_.already_num_++;
          BUF_UNLOCK();
          return 0;
        }
      }
      else {
        code_ref_temp_ = code;
      }
    }
  }
  BUF_UNLOCK();
  return -1;
}

int WTRecvPhyLayerInit()
{
  ring_buff_fd_ = RingBuffCreate(RING_BUFF_LEN);
  if (ring_buff_fd_ == NULL) {
    return -1;
  }
  BUF_LOCK_INIT();
  old_code_ref_.already_num_ = 0;
  old_code_ref_.code_num_ = 0;
  old_code_ref_.code_ = REF_CODE_INIT_DATA;
  code_ref_temp_ = REF_CODE_INIT_DATA;
  pcm_buf_w_addr_ = 0;
  return 0;
}

void WTRecvPhyLayerExit()
{
  BUF_LOCK();
  RingBuffDestroy(ring_buff_fd_);
  old_code_ref_.already_num_ = 0;
  old_code_ref_.code_num_ = 0;
  old_code_ref_.code_ = REF_CODE_INIT_DATA;
  code_ref_temp_ = REF_CODE_INIT_DATA;
  pcm_buf_w_addr_ = 0;
  BUF_UNLOCK();
  BUF_LOCK_EXIT();
}

void WTRecvPhyLayerSendPcm(const RecvAudioType * pcm, int pcm_len)
{
  int pcm_r_addr = 0;
  WTFreqCodeType code;

  if (pcm_buf_w_addr_ != 0) {
    if (pcm_len < FREQ_ANA_BUF_SIZE - pcm_buf_w_addr_) {
      memcpy(&pcm_buf_[pcm_buf_w_addr_], pcm, sizeof(RecvAudioType)*pcm_len);
      pcm_buf_w_addr_ += pcm_len;
      return;
    }
    memcpy(&pcm_buf_[pcm_buf_w_addr_], pcm, sizeof(RecvAudioType)*(FREQ_ANA_BUF_SIZE - pcm_buf_w_addr_));
    pcm_r_addr += FREQ_ANA_BUF_SIZE - pcm_buf_w_addr_;
    pcm_buf_w_addr_ = 0;
    if (WTPhysicalPcmDecode(pcm_buf_, FREQ_ANA_BUF_SIZE, &code) == 0) {
      BUF_LOCK();
      //WtLog(" %ld ", code);
      RingBuffWriteData(ring_buff_fd_, &code, sizeof(WTFreqCodeType));
      BUF_UNLOCK();
    }
  }
  while (pcm_len - pcm_r_addr >= FREQ_ANA_BUF_SIZE) {
    if (WTPhysicalPcmDecode(pcm + pcm_r_addr, FREQ_ANA_BUF_SIZE, &code) != 0) {
      pcm_r_addr += FREQ_ANA_BUF_SIZE;
      continue;
    }
    pcm_r_addr += FREQ_ANA_BUF_SIZE;
    BUF_LOCK();
    //WtLog(" %ld ", code);
    RingBuffWriteData(ring_buff_fd_, &code, sizeof(WTFreqCodeType));
    BUF_UNLOCK();
  }

  if (pcm_r_addr<pcm_len) {
    memcpy(pcm_buf_, pcm + pcm_r_addr, sizeof(RecvAudioType)*(pcm_len - pcm_r_addr));
    pcm_buf_w_addr_ = pcm_len - pcm_r_addr;
  }
}

int WTRecvPhyLayerGetData(WTFreqCodeType * data_buf, int buf_len)
{
  int i;
  for (i = 0; i < buf_len; i++) {
    if (GetNextFreqCode(&data_buf[i]) != 0) {
      break;
    }
  }
  return i;
}
