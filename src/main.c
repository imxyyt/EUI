#include "eui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define ERROR "\033[;31mERROR\033[0m: "
#define WARN  "\033[;33mWARN\033[0m: "
#define INFO  "\033[;32mINFO\033[0m: "
#define DEBUG "\033[;35mDEBUG\033[0m: "

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){//初始化
  SDL_SetAppMetadata("EUI","0.0.1","imxyyt");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log(ERROR"init: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_CreateWindowAndRenderer("window", 128, 64, 0, &window, &renderer)) {
    SDL_Log(ERROR"window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_Log(INFO"start");
  for(int i=0;i<argc;i++)SDL_Log(INFO"argv[%d]=%s",i,argv[i]);
  return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result){//销毁处理
  SDL_Log(INFO"run time %.2fs",SDL_GetTicks()/1000.0);
}
void callFun(Rect *rect,u8 buff[EUI_W][EUI_H>>3]){
  for(int i=0;i<EUI_H>>3;i++){
    for(int j=0;j<8;j++){
      for(int k=0;k<EUI_W;k++){
        if(buff[k][i]&(1<<j)){
          SDL_SetRenderDrawColorFloat(renderer, 0,0,0, 0x00);
        }else{
          SDL_SetRenderDrawColorFloat(renderer, 0xff,0xff,0xff, 0x00);
        }
        SDL_RenderPoint(renderer,k,(i<<3)+j);
      }
    }
  }
}

SDL_AppResult SDL_AppIterate(void *appstate){//刷新帧
  static Uint64 fpsStart=0;
  static Uint16 fps=1;//帧数能超65535算你牛逼
  Uint64 nowTick=SDL_GetTicks();
  SDL_SetRenderDrawColorFloat(renderer, 0xff,0xff,0xff, 0x00);
  SDL_RenderClear(renderer);
  
  eui_puts(0,0,"Hello World!");
  eui_update(callFun);
  
  SDL_RenderPresent(renderer);
  if(nowTick-fpsStart==1000){
    SDL_Log(INFO"%u fps",fps);
    fpsStart=nowTick;
    fps=0;
  }
  fps++;
  return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){//事件处理
  switch (event->type){
    case SDL_EVENT_QUIT://退出
      return SDL_APP_SUCCESS;
    
    default:
      return SDL_APP_CONTINUE;
  }
}