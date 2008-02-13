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
  initializeDrawing() ;
  // update might be needed
  updateTextBox();
  if (!checkVisibility() || isTextEmpty() )
  {
    endDrawing() ;
    return ;
  }
  
  clip();
  drawBox();
  drawTextContent();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  initializeDrawing() ;
  // update might be needed
  updateTextBox();
  if (!checkVisibility() || isTextEmpty() )
  {
    endDrawing();
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
