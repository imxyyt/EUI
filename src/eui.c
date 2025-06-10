#include "eui.h"

u8 buff[EUI_W][EUI_H>>3]={0};

static Rect updataRange={EUI_W/2,EUI_H/2,EUI_W/2,EUI_H/2};

void eui_init(){

}

u8 eui_putc(s16 x, s16 y, char c){
  if( c<EN_START_CHAR||c>EN_END_CHAR||//字符超范围
      x+EN_CHAR_W>EUI_W||y+EN_CHAR_H>EUI_H//显示区超范围
  )return 1;
  if(updataRange.x>x)updataRange.x=x;//刷新边界
  if(updataRange.y>y)updataRange.y=y;
  if(updataRange.w<x+EN_CHAR_W)updataRange.w=x+EN_CHAR_W;
  if(updataRange.h<y+EN_CHAR_H)updataRange.h=y+EN_CHAR_H;
  for(u8 i=0;i<EN_CHAR_W;i++){
    if(y&7){//如果不是整数行
      buff[x+i][y>>3]&=~(0xff<<(y&7));//清除上半部分
      buff[x+i][y>>3]|=en_char[c-EN_START_CHAR][i]<<(y&7);//绘制上半部分
      buff[x+i][(y>>3)+1]&=~(0xff>>(8-(y&7)));//清除下半部分
      buff[x+i][(y>>3)+1]|=en_char[c-EN_START_CHAR][i]>>(8-(y&7));//绘制下半部分
    }else{
      buff[x+i][y>>3]=en_char[c-EN_START_CHAR][i];//直接绘制
    }
  }
  return 0;
}

void eui_puts(s16 x, s16 y, char *str){
  while(*str){
    if(eui_putc(x,y,*str)){
      y+=EN_CHAR_H;
      x=0;
      eui_putc(x,y,*str);
    }
    x+=EN_CHAR_W+EN_CHAR_SPACE;
    str++;
  }
}

void eui_point(s16 x, s16 y){
  if(updataRange.x>x)updataRange.x=x;//刷新边界
  if(updataRange.y>y)updataRange.y=y;
  if(updataRange.w<x)updataRange.w=x;
  if(updataRange.h<y)updataRange.h=y;
  buff[x][y>>3]|=1<<(y&7);
}

void eui_line(s16 x0, s16 y0, s16 x1, s16 y1){//抄的Bresenham直线算法（
  if(x0<x1){
    updataRange.x=updataRange.x<x0?updataRange.x:x0;//刷新边界
    updataRange.w=updataRange.w>x1?updataRange.w:x1;
  }
  if(y0<y1){
    updataRange.y=updataRange.y<y0?updataRange.y:y0;
    updataRange.h=updataRange.h>y1?updataRange.h:y1;
  }
  s16 dx = x1 - x0;
  s16 dy = y1 - y0;
  s16 stepX = dx >= 0 ? 1 : -1;
  s16 stepY = dy >= 0 ? 1 : -1;
  dx = dx<0?-dx:dx;
  dy = dy<0?-dy:dy;

  if (dx > dy) { // |m| < 1，以 X 为主步进方向
    s16 p = 2 * dy - dx;
    s16 y = y0;
    for (s16 x = x0; x != x1; x += stepX) {
      if(x>=0&&x<EUI_W && y>0&&y<EUI_H)buff[x][y>>3]|=1<<(y&7);//画点
      if (p > 0) {
        y += stepY;
        p -= 2 * dx;
      }
      p += 2 * dy;
    }
  } else { // |m| >= 1，以 Y 为主步进方向
    s16 p = 2 * dx - dy;
    s16 x = x0;
    for (s16 y = y0; y != y1; y += stepY) {
      if(x>=0&&x<EUI_W && y>0&&y<EUI_H)buff[x][y>>3]|=1<<(y&7);//画点
      if (p > 0) {
        x += stepX;
        p -= 2 * dy;
      }
      p += 2 * dx;
    }
  }
  buff[x1][y1>>3]|=1<<(y1&7); // 最后补画终点
}

void eui_fill(s16 x, s16 y, s16 w, s16 h, s16 style) {
    // 更新刷新区域
    if (updataRange.x > x) updataRange.x = x;
    if (updataRange.y > y) updataRange.y = y;
    if (updataRange.w < x + w) updataRange.w = x + w;
    if (updataRange.h < y + h) updataRange.h = y + h;
}

void eui_update(updataFun fun){
  fun(&updataRange,buff);
  updataRange.x=EUI_W/2;
  updataRange.y=EUI_H/2;
  updataRange.w=EUI_W/2;
  updataRange.h=EUI_H/2;
}