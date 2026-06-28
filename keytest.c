#include <stdio.h>
#include <stdint.h>
#define EUI_KEY_LONG_TIME 3
#include "src/key.h"
int main(){
  EUI_KEY eui_key={.change=0,.history=0,.status=0,.time=0};
  printf("EUI_KEY size: %ld  MODE size:%ld\n",sizeof(eui_key),sizeof(EUI_KEY_MODE_LONG));
  char input[]="00000101011111100001111000010000";
  for(int i=0;input[i];i++){
    eui_update_key(input[i]=='1',&eui_key);
    EUI_ON(eui_key, EUI_KEY_MODE_LONG, puts("long key"));
    EUI_ON(eui_key, EUI_KEY_MODE_SHORT, puts("short key"));
    EUI_ON(eui_key, EUI_KEY_MODE_DOUBLE, puts("double key"));
    EUI_ON(eui_key, EUI_KEY_MODE_SHORT_NDOUBLE, puts("short not double key"));
    printf("mode:  %x%x%x%x %x c%d time: %d  hex: %04x\n",
      (eui_key.history&0b1000)==0b1000,
      (eui_key.history&0b0100)==0b0100,
      (eui_key.history&0b0010)==0b0010,
      (eui_key.history&0b0001)==0b0001,
      eui_key.status,
      eui_key.change,
      eui_key.time,
      eui_key
    );
  }
  return 0;
}