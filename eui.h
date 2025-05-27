#ifndef _EUI_H_
#define _EUI_H_
#include "en8x5.h"

#define EUI_W 128
#define EUI_H 64

typedef struct{
  u8 x;
  u8 y;
  u8 w;
  u8 h;
}Rect;

typedef void (*updataFun)(Rect *range, u8 buff[EUI_W][EUI_H>>3]);

extern u8 buff[EUI_W][EUI_H>>3];

extern void eui_init();
extern u8   eui_putc(u8 x, u8 y, char c);//画不下或者错误返回1
extern void eui_puts(u8 x, u8 y, char *str);
extern void eui_point(u8 x, u8 y);
extern void eui_line(u8 x, u8 y, u8 x1, u8 y1);
extern void eui_fill(u8 x, u8 y, u8 w , u8 h , u8 style);
extern void eui_update(updataFun fun);

#endif // !_EUI_H_