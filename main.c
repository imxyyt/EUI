#include <stdio.h>
#include "eui.h"

void callFun(Rect *rect,u8 buff[EUI_W][EUI_H>>3]){
  for(int i=0;i<EUI_H>>3;i++){
    for(int j=0;j<8;j++){
      for(int k=0;k<EUI_W;k++){
        printf(buff[k][i]&(1<<j)?"■":"□");
      }
      putchar('\n');
    }
  }
  // for(int i=0;i<EUI_H>>3;i++){
  //   for(int j=0;j<EUI_W;j++){
  //     printf("%02X",buff[j][i]);
  //   }
  //   putchar('\n');
  // }
}
int main(){
  eui_fill(0,0,EUI_W,EUI_H,0xf0);
  eui_update(callFun);
}