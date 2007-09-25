/*------------------------------------------------------------------------*/
/* file: DrawableText.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a text object          */
/*------------------------------------------------------------------------*/

#include "DrawableText.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableText::draw( void )
{
  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  unClip();
  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableText::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/

}
