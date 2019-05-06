/*
#
# Copyright 2018, sohu ipc
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: fawangchen@sohu-inc.com
*/
#include "parity_codec.h"

#define ODD_BOOL            (1)
#define EVEN_BOOL           (0)


int ParityEncode(ParityCodecType data, int bit_len)
{
  int one_num = 0;
  int i;
  for (i = 0; i < bit_len; i++) {
    if ((0x0001 << i)&data) {
      one_num++;
    }
  }
  if (one_num % 2) {
    return ODD_BOOL;
  }
  else {
    return EVEN_BOOL;
  }
  return 0;
}

int ParityDecode(ParityCodecType data, int bit_len, unsigned char check_data)
{
  int one_num = 0;
  int i;
  for (i = 0; i < bit_len; i++) {
    if ((0x0001 << i)&data) {
      one_num++;
    }
  }
  unsigned char temp;
  if (one_num % 2) {
    temp = ODD_BOOL;
  }
  else {
    temp = EVEN_BOOL;
  }
  if (temp != check_data) {
    return 0;
  }
  return 1;
}
