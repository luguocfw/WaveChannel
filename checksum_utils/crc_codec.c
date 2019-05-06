#include "crc_codec.h"

#ifdef __cplusplus
extern "C" {
#endif

const unsigned short w_crc_tables_[] = {
  0x0000, 0xCC01, 0xD801, 0x1400, 0xF001,
  0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00,
  0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};

unsigned short crc_16(const unsigned char *msg, int data_len)
{
  unsigned short w_crc = 0xFFFF;
  int i;
  unsigned char ch;

  for (i = 0; i<data_len; i++) {
    ch = *(msg++);
    w_crc = w_crc_tables_[(ch ^ w_crc) & 15] ^ (w_crc >> 4);
    w_crc = w_crc_tables_[((ch >> 4) ^ w_crc) & 15] ^ (w_crc >> 4);
}
  return w_crc;
}


#ifdef __cplusplus
};
#endif