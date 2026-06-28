#ifndef __EN8X5_H__
#define __EN8X5_H__

#include <stdint.h>

#define EN_CHAR_W 5
#define EN_CHAR_H 8
#define EN_START_CHAR ' '
#define EN_END_CHAR   '~'
#define EN_NUM_CHAR   (EN_END_CHAR - EN_START_CHAR + 1)
extern const uint8_t en_char[EN_NUM_CHAR][EN_CHAR_W];

#define en8x5(c) ((c>=EN_START_CHAR && c<=EN_END_CHAR)?en_char[c-EN_START_CHAR]:en_char[' '-EN_START_CHAR])

#endif