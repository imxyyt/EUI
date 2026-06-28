#include <stdio.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;


char EUI_STR_BUF[17]={0};
#define EUI_STR_BUF_START (EUI_STR_BUF)
#define EUI_STR_BUF_U8BIN (EUI_STR_BUF+8)
#define EUI_STR_BUF_U8HEX (EUI_STR_BUF+14)
#define EUI_STR_BUF_U16HEX (EUI_STR_BUF+12)
#define EUI_STR_BUF_U32HEX (EUI_STR_BUF+8)
#define EUI_STR_BUF_END (EUI_STR_BUF+16)
char* eui_u2bstr(u16 num){//u16转二进制字符串
  char* ret=EUI_STR_BUF_END;
  while(ret!=EUI_STR_BUF){
    *--ret=(num&1u)+'0';
    num>>=1;
  }
  return ret;
}
char* eui_u2hstr(u32 num){//u32转16进制字符串
  char* ret=EUI_STR_BUF_END;
  while(ret!=EUI_STR_BUF_U32HEX){
    ret--;
    if((num&0x0F)<10)*ret=(num&0x0F)+'0';
    else *ret=(num&0x0F)-10+'A';
    num>>=4;
  }
  return ret;
}
char* eui_u2str(u32 num){//u32转10进制字符串
  char* ret=EUI_STR_BUF_END;
  while(num){
    ret--;
    *ret=(num%10)+'0';
    num/=10;
  }
  return ret;
}
char* eui_i2str(s32 num){//i32转10进制字符串
  char* ret;
  if(num<0){
    ret=eui_u2str(-num);
    *--ret='-';
  }else{
    ret=eui_u2str(num);
  }
  return ret;
}

int main(){
  printf("s=%s\n",eui_i2str(-123456789));
  return 0;
}