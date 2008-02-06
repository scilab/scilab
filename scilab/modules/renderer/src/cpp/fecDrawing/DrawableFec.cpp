/*------------------------------------------------------------------------*/
/* file: DrawableFec.cpp                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a fec object           */
/*------------------------------------------------------------------------*/

#include "DrawableFec.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableFec::draw( void )
{
  if (!checkVisibility())
  {
    return;
  }
  initializeDrawing();
  clip();
  drawFec();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableFec::show( void )
{
  if (!checkVisibility())
  {
    return;
  }
  initializeDrawing();
  clip();
  showFec();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
