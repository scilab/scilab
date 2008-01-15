/*------------------------------------------------------------------------*/
/* file: DrawableText.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a text object          */
/*------------------------------------------------------------------------*/

#include "DrawableText.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableText::draw( void )
{
  if (!checkVisibility() || isTextEmpty() )
  {
    return ;
  }
  initializeDrawing() ;
  clip();
  drawBox();
  drawTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  if (!checkVisibility() || isTextEmpty() )
  {
    return ;
  }
  initializeDrawing();
  clip();
  showBox();
  showTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
