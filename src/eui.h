#ifndef _EUI_H_
#define _EUI_H_
#include "fonts/en8x5.h"

#define EUI_W 128
#define EUI_H 64

typedef struct{
  s16 x;
  s16 y;
  s16 w;
  s16 h;
}Rect;

typedef void (*updataFun)(Rect *range, u8 buff[EUI_W][EUI_H>>3]);

extern u8 buff[EUI_W][EUI_H>>3];

extern void eui_init();
extern u8   eui_putc(s16 x, s16 y, char c);//画不下或者错误返回1
extern void eui_puts(s16 x, s16 y, char *str);
extern void eui_point(s16 x, s16 y);
extern void eui_line(s16 x0, s16 y0, s16 x1, s16 y1);
extern void eui_fill(s16 x, s16 y, s16 w , s16 h , s16 style);
extern void eui_update(updataFun fun);

#endif // !_EUI_H_