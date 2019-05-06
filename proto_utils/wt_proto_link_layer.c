/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include "wt_proto_link_layer.h"
#include "checksum_utils/crc_codec.h"
#include "rs_code.h"
#include <stdlib.h>
#include <string.h>

#define RS_SYMSIZE			8
#define RS_GFPOLY			0x11d
#define RS_FCR				1
#define RS_PRIM				1


static WTFreqCodeType freq_code_st_mark_[COMPARE_FREQ_ST_NUM] = COMPARE_ST_CODE;

int WTLinkCheckStCode(unsigned char *code, int len)
{
  if (len != COMPARE_FREQ_ST_NUM) {
    return 0;
  }
  int i;
  unsigned temp;
  for (i = 0; i < len; i++) {
    temp = (code[i] >> COMPARE_DATA_NUM_LEN);
    if (temp != freq_code_st_mark_[i]) {
      return 0;
    }
  }
  return 1;
}

int WTLinkChecksumDecode(WaveTransLinkPackage * package)
{
  void *rs_hander = NULL;
  int eras_pos[COMPARE_FREQ_DATA_NUM + COMPARE_FREQ_CHECKSUM_NUM];
  unsigned char buff[COMPARE_FREQ_DATA_NUM + COMPARE_FREQ_CHECKSUM_NUM] = { 0 };
  memset(eras_pos, 0, sizeof(int)*(COMPARE_FREQ_DATA_NUM + COMPARE_FREQ_CHECKSUM_NUM));
  rs_hander = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, COMPARE_FREQ_CHECKSUM_NUM, ((1 << RS_SYMSIZE) - 1 - (package->real_data_num_ + COMPARE_FREQ_CHECKSUM_NUM)));
  if (rs_hander == NULL) {
    return 1;
  }
  memcpy(buff, package->byte_data_, package->real_data_num_);
  memcpy(buff + package->real_data_num_, &package->check_sum_, COMPARE_FREQ_CHECKSUM_NUM);
  decode_rs_char(rs_hander, buff, eras_pos, 0);
  memcpy(package->byte_data_, buff, package->real_data_num_);
  free_rs_cache();
  return 1;
}

void WTLinkChecksumEncode(WaveTransLinkPackage * package)
{
  void *rs_hander = NULL;
  rs_hander = init_rs(RS_SYMSIZE, RS_GFPOLY, RS_FCR, RS_PRIM, COMPARE_FREQ_CHECKSUM_NUM,
    ((1 << RS_SYMSIZE) - 1 - (package->real_data_num_ + COMPARE_FREQ_CHECKSUM_NUM)));
  if (rs_hander == NULL) {
    return;
  }
  encode_rs_char(rs_hander, (data_t *)package->byte_data_, (data_t *)(&package->check_sum_));
  free_rs_cache();
}

void WTLinkPackageEncode(const WaveTransLinkPackage * package, WaveTransPhyPackage * phy_pack)
{
  int i;
  unsigned char data_len = (unsigned char)package->real_data_num_;
  for (i = 0; i < COMPARE_FREQ_ST_NUM; i++) {
    phy_pack->st_mark_[i] = freq_code_st_mark_[i];
    phy_pack->st_mark_[i] <<= COMPARE_DATA_NUM_LEN;
    phy_pack->st_mark_[i] += data_len - 1;
  }
  memset(phy_pack->byte_data_, 0, sizeof(WTFreqCodeType)*COMPARE_FREQ_DATA_NUM);
  for (i = 0; i < package->real_data_num_; i++) {
    phy_pack->byte_data_[i] = package->byte_data_[i];
  }
  for (i = 0; i < COMPARE_FREQ_CHECKSUM_NUM; i++) {
    phy_pack->check_byte_data_[i] = package->check_sum_[i];
  }
}

void WTLinkPackageDecode(const WaveTransPhyPackage * package, WaveTransLinkPackage * link_pack)
{
  int i;
  for (i = 0; i < COMPARE_FREQ_ST_NUM; i++) {
    link_pack->real_data_num_ = (0x0003 & package->st_mark_[i]) + 1;
  }
  for (i = 0; i < link_pack->real_data_num_; i++) {
    link_pack->byte_data_[i] = (unsigned char)package->byte_data_[i];
  }
  for (i = 0; i < COMPARE_FREQ_CHECKSUM_NUM; i++) {
    link_pack->check_sum_[i] = (unsigned char)package->check_byte_data_[i];
  }
}


void WTLinkFreqCodeWriteToPhyPackage(WTFreqCodeType code, WaveTransPhyPackage *package)
{
  int i;
  int j = 0;
  for (i = 0; i < COMPARE_FREQ_CHECKSUM_NUM; i++) {
    package->check_byte_data_[i] <<= 1;
    if (code&(0x0001 << j)) {
      package->check_byte_data_[i] |= 0x0001;
    }
    j++;
    package->check_byte_data_[i] &= ~(0x0001 << COMPARE_FREQ_BIT);
  }
  for (i = 0; i < COMPARE_FREQ_DATA_NUM; i++) {
    package->byte_data_[i] <<= 1;
    if (code&(0x0001 << j)) {
      package->byte_data_[i] |= 0x0001;
    }
    j++;
    package->byte_data_[i] &= ~(0x0001 << COMPARE_FREQ_BIT);
  }
  for (i = 0; i < COMPARE_FREQ_ST_NUM; i++) {
    package->st_mark_[i] <<= 1;
    if (code&(0x0001 << j)) {
      package->st_mark_[i] |= 0x0001;
    }
    j++;
    package->st_mark_[i] &= ~(0x0001 << COMPARE_FREQ_BIT);
  }

}

int WTLinkFreqCodeReadFromPhyPackage(const WaveTransPhyPackage * package, WTFreqCodeType * code, int code_len)
{
  if (code_len != COMPARE_FREQ_DATA_BIT) {
    return -1;
  }
  memset(code, 0, sizeof(WTFreqCodeType)*COMPARE_FREQ_DATA_BIT);
  int i;
  int j;
  for (j = 0; j < COMPARE_FREQ_DATA_BIT; j++) {
    for (i = 0; i < COMPARE_FREQ_CHECKSUM_NUM; i++) {
      if (package->check_byte_data_[i] & (0x0001 << j)) {
        code[j] |= (0x0001 << i);
      }
    }
    for (i = 0; i < COMPARE_FREQ_DATA_NUM; i++) {
      if (package->byte_data_[i] & (0x0001 << j)) {
        code[j] |= (0x0001 << (i + COMPARE_FREQ_CHECKSUM_NUM));
      }
    }
    for (i = 0; i < COMPARE_FREQ_ST_NUM; i++) {
      if (package->st_mark_[i] & (0x0001 << j)) {
        code[j] |= (0x0001 << (i + COMPARE_FREQ_CHECKSUM_NUM + COMPARE_FREQ_DATA_NUM));
      }
    }
  }
  return 0;
}
