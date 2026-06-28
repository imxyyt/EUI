#include "eui.h"

uint8_t EUI_buf[EUI_H>>3][EUI_W]={0};
char EUI_STR_BUF[17]={'0'};
int8_t _EUI_INTERVAL=ECHAR_W+1;
uint8_t _EUI_FOCUS_X;
uint8_t _EUI_FOCUS_Y;
uint8_t _EUI_FOCUS_Y_L;
uint8_t _EUI_FOCUS_Y_H;
void eui_putc(char c){
  if(_EUI_FOCUS_X+ECHAR_W>EUI_W||_EUI_FOCUS_Y+ECHAR_H>EUI_H)return;
  const uint8_t *show = ECHAR_GET(c);
  for(uint8_t w=0;w<ECHAR_W;w++){
    EUI_buf[_EUI_FOCUS_Y_H][_EUI_FOCUS_X+w]|=show[w]<<(_EUI_FOCUS_Y_L);//上半部分
    EUI_buf[(_EUI_FOCUS_Y_H)+1][_EUI_FOCUS_X+w]|=show[w]>>(8-(_EUI_FOCUS_Y_L));//下半部分
  }
}

void eui_puts(uint8_t x, uint8_t y, char *str){
  eui_set_focus(x,y);
  while(*str){
    eui_putc(*str);
    switch(*str){
      case '\n':
        _EUI_FOCUS_Y+=ECHAR_H;
        _EUI_FOCUS_Y_H = _EUI_FOCUS_Y >> 3;
        _EUI_FOCUS_Y_L = _EUI_FOCUS_Y & 0x07;
        break;
      case '\r':
        _EUI_FOCUS_X=x;
        break;
      default:
        _EUI_FOCUS_X+=_EUI_INTERVAL;
    }
    str++;
  }
}
void eui_line(uint8_t x, uint8_t y){
  // Bresenham Line Algorithm from https://www.edepot.com/linebresenham.html
  if(_EUI_FOCUS_X>=EUI_W)_EUI_FOCUS_X=EUI_W-1;
  if(_EUI_FOCUS_Y>=EUI_H){
    _EUI_FOCUS_Y=EUI_H-1;
    _EUI_FOCUS_Y_H = _EUI_FOCUS_Y >> 3;
    _EUI_FOCUS_Y_L = _EUI_FOCUS_Y & 0x07;
  }
  if(x>=EUI_W)x=EUI_W-1;
  if(y>=EUI_H)y=EUI_H-1;
  uint8_t  dx, dy;
  int8_t incx, incy;
  int16_t  balance;//int8_t范围可能小于128

  if (x >= _EUI_FOCUS_X){
    dx = x - _EUI_FOCUS_X;
    incx = 1;
  }else{
    dx = _EUI_FOCUS_X - x;
    incx = -1;
  }
  if (y >= _EUI_FOCUS_Y){
    dy = y - _EUI_FOCUS_Y;
    incy = 1;
  }else{
    dy = _EUI_FOCUS_Y - y;
    incy = -1;
  }
  if (dx >= dy){
    dy <<= 1;
    balance = dy - dx;
    dx <<= 1;
    while (_EUI_FOCUS_X != x){
      eui_point();
      if (balance >= 0){
        _EUI_FOCUS_Y += incy;
        balance -= dx;
      }
      balance += dy;
      _EUI_FOCUS_X += incx;
    }
    eui_point();
  }else{
    dx <<= 1;
    balance = dx - dy;
    dy <<= 1;
    while (_EUI_FOCUS_Y != y){
      eui_point();;
      if (balance >= 0){
        _EUI_FOCUS_Y += incx;
        balance -= dy;
      }
      balance += dx;
      _EUI_FOCUS_Y += incy;
    }
    eui_point();
  }
}

void eui_rect(uint8_t w , uint8_t h , uint8_t style){
  uint8_t i,j;
  if(_EUI_FOCUS_X+w>=EUI_W||_EUI_FOCUS_Y+h>=EUI_H)return;
  for(i=_EUI_FOCUS_X;i<_EUI_FOCUS_X+w ;i++){
    EUI_buf[_EUI_FOCUS_Y_H][i] |= 1<<(_EUI_FOCUS_Y_L);//上边缘
    EUI_buf[_EUI_FOCUS_Y_H][i] |= (style<<(_EUI_FOCUS_Y_L));//上边缘填充
    for(j=_EUI_FOCUS_Y_H+1; j!=(_EUI_FOCUS_Y+h)>>3; j++)EUI_buf[j][i]|=style;//中间部分填充
    EUI_buf[(_EUI_FOCUS_Y+h)>>3][i] |= style>>(8-(_EUI_FOCUS_Y_L));//下边缘填充
    EUI_buf[(_EUI_FOCUS_Y+h)>>3][i] |= 1<<((_EUI_FOCUS_Y+h)&0b111);//下边缘
  }
  EUI_buf[_EUI_FOCUS_Y_H][_EUI_FOCUS_X] |= (0xff<<(_EUI_FOCUS_Y_L));//左上边缘填充
  EUI_buf[_EUI_FOCUS_Y_H][_EUI_FOCUS_X+w-1] |= (0xff<<(_EUI_FOCUS_Y_L));//右上边缘填充
  EUI_buf[(_EUI_FOCUS_Y+h)>>3][_EUI_FOCUS_X] |= 0xff>>(8-((_EUI_FOCUS_Y+h)&0x07));//左下边缘填充
  EUI_buf[(_EUI_FOCUS_Y+h)>>3][_EUI_FOCUS_X+w-1] |= 0xff>>(8-((_EUI_FOCUS_Y+h)&0x07));//右下边缘填充
  for(j=(_EUI_FOCUS_Y_H+1); j!=(_EUI_FOCUS_Y+h)>>3; j++){
    EUI_buf[j][_EUI_FOCUS_X] = 0xff;//左右两边边缘
    EUI_buf[j][_EUI_FOCUS_X+w-1] = 0xff;
  }
}

void eui_circle(uint8_t r, uint8_t style){
  if(_EUI_FOCUS_X+r>=EUI_W||_EUI_FOCUS_Y+r>=EUI_H||_EUI_FOCUS_X-r<0||_EUI_FOCUS_Y-r<0)return;
  uint8_t x=0, y=r, i;
  int16_t d=1-r;
  #define drawPixel(x, y)EUI_buf[(_EUI_FOCUS_Y+y)>>3][_EUI_FOCUS_X+x]|=1<<((_EUI_FOCUS_Y+y)&0x07)
  while (x < y) {
    drawPixel( x, y); drawPixel( y, x);   // 八分画圆
    drawPixel( x,-y); drawPixel(-y, x);
    drawPixel(-x, y); drawPixel( y,-x);
    drawPixel(-x,-y); drawPixel(-y,-x);
    if(style){
      i=0xff<<((_EUI_FOCUS_Y-y)&0x07);
      EUI_buf[(_EUI_FOCUS_Y-y)>>3][_EUI_FOCUS_X+x] |= style&i;//顶部
      EUI_buf[(_EUI_FOCUS_Y-y)>>3][_EUI_FOCUS_X-x] |= style&i;
      i=0xff<<((_EUI_FOCUS_Y-x)&0x07);
      EUI_buf[(_EUI_FOCUS_Y-x)>>3][_EUI_FOCUS_X+y] |= style&i;
      EUI_buf[(_EUI_FOCUS_Y-x)>>3][_EUI_FOCUS_X-y] |= style&i;
      for(i=((_EUI_FOCUS_Y-y)>>3)+1;i!=(_EUI_FOCUS_Y+y)>>3;i++){//中间部分填充
        EUI_buf[i][_EUI_FOCUS_X+x] |= style;
        EUI_buf[i][_EUI_FOCUS_X-x] |= style;
      }
      for(i=((_EUI_FOCUS_Y-x)>>3)+1;i<(_EUI_FOCUS_Y+x)>>3;i++){//两侧部分填充
        EUI_buf[i][_EUI_FOCUS_X+y] |= style;
        EUI_buf[i][_EUI_FOCUS_X-y] |= style;
      }
      i=0xff>>(8-((_EUI_FOCUS_Y+y)&0x07));
      EUI_buf[(_EUI_FOCUS_Y+y)>>3][_EUI_FOCUS_X+x] |= style&i;//底部
      EUI_buf[(_EUI_FOCUS_Y+y)>>3][_EUI_FOCUS_X-x] |= style&i;
      i=0xff>>(8-((_EUI_FOCUS_Y+x)&0x07));
      EUI_buf[(_EUI_FOCUS_Y+x)>>3][_EUI_FOCUS_X+y] |= style&i;
      EUI_buf[(_EUI_FOCUS_Y+x)>>3][_EUI_FOCUS_X-y] |= style&i;
    }
    if(d < 0) d += (x<<2) + 1;
    else{
      y--;
      d += ((x - y)<<2) + 1;
    }
    x++;
  }
}
void eui_move_left(uint8_t x){//屏幕左移x个像素
  for(uint8_t h=0;h<(EUI_H>>3);h++){
    for(uint8_t w=0;w<(EUI_W-x);w++)
      EUI_buf[h][w]=EUI_buf[h][w+x];
  }
}
void eui_move_right(uint8_t x){//屏幕右移x个像素
  for(uint8_t h=0;h<(EUI_H>>3);h++){
    for(uint8_t w=EUI_W-1;w>=x;w--)
      EUI_buf[h][w]=EUI_buf[h][w-x];
  }
}
void eui_clear(uint8_t style){
  for(uint8_t y=0;y<(EUI_H>>3);y++)
    for(uint8_t x=0;x<EUI_W;x++)
      EUI_buf[y][x]=style;
}

//数字转字符
char* eui_u2bstr(uint16_t num){//u16转二进制字符串
  char* ret=EUI_STR_BUF_END;
  while(ret!=EUI_STR_BUF){
    *--ret=(num&1u)+'0';
    num>>=1;
  }
  return ret;
}
char* eui_u2hstr(uint32_t num){//uint32_t转16进制字符串
  char* ret=EUI_STR_BUF_END;
  while(ret!=EUI_STR_BUF_U32HEX){
    ret--;
    if((num&0x0F)<10)*ret=(num&0x0F)+'0';
    else *ret=(num&0x0F)-10+'A';
    num>>=4;
  }
  return ret;
}
char* eui_u2str(uint32_t num){//uint32_t转10进制字符串
  char* ret=EUI_STR_BUF_END;
  do{
    ret--;
    *ret=(num%10)+'0';
    num/=10;
  }while(num);
  return ret;
}
char* eui_i2str(int32_t num){//i32转10进制字符串
  char* ret;
  if(num<0){
    ret=eui_u2str(-num);
    *--ret='-';
  }else{
    ret=eui_u2str(num);
  }
  return ret;
}