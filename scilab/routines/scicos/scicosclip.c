#include "../machine.h"


void C2F(scicosclip)(int *n)
{
  if(*n==1){
frame_clip_on();
  }
  else{
frame_clip_off();
  }
}
