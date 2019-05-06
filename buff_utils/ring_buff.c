/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include "ring_buff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int             read_addr_;
  int             write_addr_;
  int             en_read_len_;
  unsigned char   *buff_;
  int             buff_len_;
}RingBuffData;


static int CheckIsWriteCover(RingBuffData *fd_data, int write_len)
{
  if (fd_data->write_addr_ >= fd_data->read_addr_) {
    if (fd_data->buff_len_ - (fd_data->write_addr_ - fd_data->read_addr_) - 1 < write_len) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    if (fd_data->read_addr_ - fd_data->write_addr_ - 1 < write_len) {
      return 1;
    }
    else {
      return 0;
    }
  }
}

RingBuffFd * RingBuffCreate(int byte_len)
{
  RingBuffData *data = (RingBuffData *)malloc(sizeof(RingBuffData));
  if (data == NULL) {
    return NULL;
  }
  data->buff_ = (unsigned char *)malloc(sizeof(unsigned char)*byte_len);
  if (data->buff_ == NULL) {
    free(data);
    return NULL;
  }
  data->buff_len_ = byte_len;
  memset(data->buff_, 0, sizeof(unsigned char)*data->buff_len_);
  data->read_addr_ = 0;
  data->write_addr_ = 0;
  data->en_read_len_ = 0;
  return (RingBuffFd *)data;
}

void RingBuffDestroy(RingBuffFd * fd)
{
  RingBuffData *data = (RingBuffData *)fd;
  free(data->buff_);
  free(data);
}

int RingBuffWriteData(RingBuffFd *fd, const void *data, int len)
{
  RingBuffData *fd_data = (RingBuffData *)fd;
  int cover_flag = CheckIsWriteCover(fd_data, len);
  if ((fd_data->buff_len_ - fd_data->write_addr_) > len) {
    memcpy(&fd_data->buff_[fd_data->write_addr_],data, len);
    fd_data->write_addr_ += len;
  }
  else {
    int temp = fd_data->buff_len_ - fd_data->write_addr_;
    memcpy(&fd_data->buff_[fd_data->write_addr_],data, temp);
    memcpy(&fd_data->buff_[0],((unsigned char *)data + temp), len - temp);
    fd_data->write_addr_ = len - temp;
  }
  if (cover_flag == 1) {
    if (fd_data->write_addr_ == fd_data->buff_len_ - 1) {
      fd_data->read_addr_ = 1;
    }
    else {
      fd_data->read_addr_ = fd_data->write_addr_ + 1;
    }
  }
  return len;
}

int RingBuffReadData(RingBuffFd *fd, void *data, int len)
{
  RingBuffData *fd_data = (RingBuffData *)fd;
  if (fd_data->read_addr_ == fd_data->write_addr_) {
    return 0;
  }
  if (fd_data->read_addr_ > fd_data->write_addr_) {
    int right_len = fd_data->buff_len_ - fd_data->read_addr_;
    if (right_len > len) {
      memcpy(data, &fd_data->buff_[fd_data->read_addr_], len);
      fd_data->read_addr_ += len;
      return len;
    }
    else {
      memcpy(data, &fd_data->buff_[fd_data->read_addr_], right_len);
      fd_data->read_addr_ = 0;
      int left_len = fd_data->write_addr_;
      if (left_len < len - right_len) {
        memcpy((unsigned char *)data + right_len, &fd_data->buff_[0], left_len);
        fd_data->read_addr_ = left_len;
        return left_len+right_len;
      }
      else {
        memcpy(((unsigned char *)data + right_len), &fd_data->buff_[fd_data->read_addr_], len - right_len);
        fd_data->read_addr_ = len - right_len;
        return len;
      }
    }
  }
  else {
    int en_read_len = fd_data->write_addr_ - fd_data->read_addr_;
    if (en_read_len > len) {
      memcpy(data, &fd_data->buff_[fd_data->read_addr_], len);
      fd_data->read_addr_ += len;
      return len;
    }
    else {
      memcpy(data, &fd_data->buff_[fd_data->read_addr_], en_read_len);
      fd_data->read_addr_ += en_read_len;
      return en_read_len;
    }
  }
}
