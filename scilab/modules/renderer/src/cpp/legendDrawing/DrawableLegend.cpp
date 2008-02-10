/*------------------------------------------------------------------------*/
/* file: DrawableLegend.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a legend object        */
/*------------------------------------------------------------------------*/

#include "DrawableLegend.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableLegend::draw( void )
{
  if (!checkVisibility())
  {
    return;
  }
  drawLegend();
}
/*---------------------------------------------------------------------------------*/
void DrawableLegend::show( void )
{
  if (!checkVisibility())
  {
    return;
  }
  showLegend();
}
/*---------------------------------------------------------------------------------*/


}
