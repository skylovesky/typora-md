#include <stdio.h>
#include <stdint.h>

/******************************************************************************
 * Name:    CRC-16/CCITT        x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Alias:   CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT
 *****************************************************************************/
uint16_t crc16_ccitt(uint8_t *data, uint16_t length)
{
  uint8_t i;
  uint16_t crc = 0; // Initial value
  while (length--)
  {
    crc ^= *data++; // crc ^= *data; data++;
    for (i = 0; i < 8; ++i)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ 0x8408; // 0x8408 = reverse 0x1021
      else
        crc = (crc >> 1);
    }
  }
  return crc;
}

int main(int argc, const char *argv[])
{
  uint32_t id = 0;

  printf("请输入十六进制ID：\n");
  scanf("%X", &id);

  printf("验证码为: %04X\n", crc16_ccitt((uint8_t*)&id, 4)); 

  return 0;
}
