/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2004 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *--------------------------------------------------------------------------*/

#include "nsp/math.h"
#include "nsp/graphics/Graphics.h"

  
void scicos_clip(BCG *Xgc,int n) 
{
  if ( n== 1) 
    frame_clip_on(Xgc);
  else
    frame_clip_off(Xgc);
}
