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

#include "checksum_utils/rs_code.h"
#define RS_SYMSIZE			8
#define RS_GFPOLY			0x11d
#define RS_FCR				1
#define RS_PRIM				1


int main()
{
  void *rs_hander = NULL;
  char test_str[9] = { 0,1,2,1,3 };
  char checksum[4] = { 0 };
  rs_hander = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM,4, ((1 << RS_SYMSIZE) - 1 - (4 + 4)));
  if (rs_hander == NULL) {
    printf("init rs hander failed\n");
    return 1;
  }
  printf("%d %d %d %d %d %d %d %d %d\n", test_str[0], test_str[1], test_str[2], test_str[3], test_str[4], test_str[5], test_str[6], test_str[7], test_str[8]);
  printf("%d %d %d %d\n", checksum[0], checksum[1], checksum[2], checksum[3]);
  encode_rs_char(rs_hander, test_str, checksum);
  printf("%d %d %d %d %d %d %d %d %d\n", test_str[0], test_str[1], test_str[2], test_str[3], test_str[4], test_str[5], test_str[6], test_str[7], test_str[8]);
  printf("%d %d %d %d\n", checksum[0], checksum[1], checksum[2], checksum[3]);
  int eras_pos[8];
  char buff[8] = { 0 };
  test_str[0] = 0x0a;
  test_str[1] = 0xaa;
  memcpy(buff, test_str, 4);
  memcpy(buff + 4, checksum, 4);
  int ret = decode_rs_char(rs_hander, buff, eras_pos, 0);
  printf("ret:%d\n", ret);
  int i;
  for (i = 0; i < 8; i++) {
    printf("%d ", buff[i]);
  }
  printf("\n");
  return 0;
}