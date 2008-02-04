/*------------------------------------------------------------------------*/
/* file: DrawableGrayplot.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a grayplot object      */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplot.h"


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableGrayplot::draw( void )
{
  if(!checkVisibility())
  {
    return ;
  }
  initializeDrawing();
  clip();
  drawGrayplot();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplot::show( void )
{
  showGrayplot();
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotBridge * DrawableGrayplot::getGrayplotImp( void )
{
  return dynamic_cast<DrawableGrayplotBridge *>(getDrawableImp());
}
/*---------------------------------------------------------------------------------*/
}
