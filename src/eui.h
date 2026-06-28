#ifndef _EUI_H_
#define _EUI_H_
#include <stdint.h>
#include "fonts/en8x5.h"
#include "key.h"

//MAX SIZE 255*255
#define EUI_W 128
#define EUI_H 64

#define ECHAR_H     EN_CHAR_H
#define ECHAR_W     EN_CHAR_W
#define ECHAR_NUM   EN_CHAR_NUM
#define ECHAR_START EN_CHAR_START
#define ECHAR_END   EN_CHAR_END
#define ECHAR_GET(c) en8x5(c)

extern char EUI_STR_BUF[17];
extern int8_t _EUI_INTERVAL;
extern uint8_t EUI_buf[EUI_H>>3][EUI_W];
extern uint8_t _EUI_FOCUS_X;
extern uint8_t _EUI_FOCUS_Y;
extern uint8_t _EUI_FOCUS_Y_H;
extern uint8_t _EUI_FOCUS_Y_L;

//设置对齐 默认 ECHAR_W+1
#define eui_set_align(align) {_EUI_INTERVAL = (align);}


#define EUI_STR_BUF_START (EUI_STR_BUF)
#define EUI_STR_BUF_U8BIN (EUI_STR_BUF+8)
#define EUI_STR_BUF_U8HEX (EUI_STR_BUF+14)
#define EUI_STR_BUF_U16HEX (EUI_STR_BUF+12)
#define EUI_STR_BUF_U32HEX (EUI_STR_BUF+8)
#define EUI_STR_BUF_END (EUI_STR_BUF+16)

//设置焦点
#define eui_set_focus(x, y) {\
  _EUI_FOCUS_X=(x);\
  _EUI_FOCUS_Y = (y);\
  _EUI_FOCUS_Y_H = _EUI_FOCUS_Y >> 3;\
  _EUI_FOCUS_Y_L = _EUI_FOCUS_Y & 0x07;\
}
//画点
#define eui_point() {\
  _EUI_FOCUS_Y_H = _EUI_FOCUS_Y >> 3;\
  _EUI_FOCUS_Y_L = _EUI_FOCUS_Y & 0x07;\
  EUI_buf[_EUI_FOCUS_Y_H][_EUI_FOCUS_X]|=1<<(_EUI_FOCUS_Y_L);\
}
extern void eui_putc(char c);//从焦点输出一个字符
extern void eui_puts(uint8_t x, uint8_t y, char *str);//从指定位置输出字符串
extern void eui_line(uint8_t x, uint8_t y);//从焦点到目标点画线
extern void eui_rect(uint8_t w , uint8_t h , uint8_t style);//从焦点画矩形
extern void eui_circle(uint8_t r, uint8_t style);//从焦点画圆
extern void eui_move_left(uint8_t x);//屏幕左移x个像素
extern void eui_move_right(uint8_t x);//屏幕右移x个像素
// extern void eui_move_up(uint8_t y);//屏幕上移y个像素
// extern void eui_move_down(uint8_t y);//屏幕下移y个像素
extern void eui_clear(uint8_t style);//按照样式清屏
//字符串转换
extern char* eui_u2bstr(uint16_t num);//uint16_t转二进制字符串
extern char* eui_u2hstr(uint32_t num);//uint32_t转16进制字符串
extern char* eui_u2str(uint32_t num);//uint32_t转10进制字符串
extern char* eui_i2str(int32_t num);//i32转10进制字符串


#endif // _EUI_H_
