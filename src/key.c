#include "key.h"

void eui_update_key(uint8_t keyStatus,EUI_KEY*Key){
  if((keyStatus&&1u)^Key->status){//如果按键状态切换
    Key->change=1u;//切换位 置1
    Key->status^=1u;//状态位翻转
    Key->history<<=1;//历史位滚动
    Key->time=EUI_KEY_LONG_TIME;//重置长按时间
  }else{
    Key->change=0;//切换位 置0
    if(Key->time)Key->time--;//如果没到长按时间，就计时
    else Key->history|=1u;//长按了就把当前历史置1
  }
}
/*
typedef struct PACK{
  uint8_t mask:2;//掩码 11一位 10两位 01三位 00四位
  uint8_t mode:1;//是否循环触发
  uint8_t status:1;//按键状态
  uint8_t history:4;//历史状态
}EUI_KEY_MODE;
*/
const EUI_KEY_MODE EUI_KEY_MODE_LONG=
  {.mask=3,.mode=1,.status=1u,.history=0b0001};
const EUI_KEY_MODE EUI_KEY_MODE_SHORT=
  {.mask=2,.mode=0,.status=0u,.history=0b0000};
const EUI_KEY_MODE EUI_KEY_MODE_DOUBLE=
  {.mask=0,.mode=0,.status=1u,.history=0b1000};
const EUI_KEY_MODE EUI_KEY_MODE_SHORT_NDOUBLE=
  {.mask=1,.mode=0,.status=0u,.history=0b0100};