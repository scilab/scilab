/*------------------------------------------------------------------------*/
/* file: DrawableAxes.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an axes object         */
/*------------------------------------------------------------------------*/

#include "DrawableAxes.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableAxes::draw( void )
{
  if (!checkVisibility())
  {
    return;
  }
  initializeDrawing();
  clip();
  drawAxes();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableAxes::show( void )
{
  if (!checkVisibility())
  {
    return;
  }
  initializeDrawing();
  clip();
  showAxes();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
