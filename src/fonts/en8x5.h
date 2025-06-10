#ifndef __EN8X5_H__
#define __EN8X5_H__

#define EN_CHAR_W 5
#define EN_CHAR_H 8
#define EN_CHAR_SPACE 1
#define EN_START_CHAR ' '
#define EN_END_CHAR   '~'

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern const u8 en_char[96][EN_CHAR_W];

#endif
