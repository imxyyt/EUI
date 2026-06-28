#ifndef EUI_KEY_H
#define EUI_KEY_H

#include <stdint.h>

// 检测大小端
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define IS_BIG_ENDIAN 1
#else
  #define IS_BIG_ENDIAN 0
#endif

#define PACK  __attribute__((packed))

typedef struct PACK{
  union PACK{
    struct PACK{
      #if IS_BIG_ENDIAN
        uint16_t history:4;//历史状态
        uint16_t status:1;//按键状态
        uint16_t change:1;//是否变化
      #else
        uint16_t change:1;//是否变化
        uint16_t status:1;//按键状态
        uint16_t history:4;//历史状态
      #endif
    };
    uint16_t mode:6;
  };
  uint16_t time:8; //状态持续时间
}EUI_KEY;
typedef struct PACK{
  uint8_t mask:2;//掩码 11一位 10两位 01三位 00四位
  uint8_t mode:1;//是否循环触发
  uint8_t status:1;//按键状态
  uint8_t history:4;//历史状态
}EUI_KEY_MODE;

extern const EUI_KEY_MODE EUI_KEY_MODE_LONG;
extern const EUI_KEY_MODE EUI_KEY_MODE_SHORT;
extern const EUI_KEY_MODE EUI_KEY_MODE_DOUBLE;
extern const EUI_KEY_MODE EUI_KEY_MODE_SHORT_NDOUBLE;

#ifndef EUI_KEY_LONG_TIME
#define EUI_KEY_LONG_TIME 80u
#endif
void eui_update_key(uint8_t keyStatus,EUI_KEY*Key);
// 如果按键变了或者是循环触发模式，并且按键状态一致，历史状态也一致，则运行
#define EUI_ON(key,Mode,run){\
  if((key.change||Mode.mode)\
    &&(key.status==Mode.status)\
    &&((key.history&(0b1111>>Mode.mask))==Mode.history)\
  ){run;}\
}

#endif