/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __PARITY_CODEC_H__
#define __PARITY_CODEC_H__

typedef unsigned short ParityCodecType;



int ParityEncode(ParityCodecType data, int bit_len);

/*reutrn : check ok 1,check failed 0*/
int ParityDecode(ParityCodecType data, int bit_len, unsigned char check_data);


#endif