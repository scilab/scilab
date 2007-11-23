/*------------------------------------------------------------------------*/
/* file: DrawableCompound.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a compound object      */
/*------------------------------------------------------------------------*/


#include "DrawableCompound.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableCompound::draw( void )
{
  if ( !checkVisibility())
  {
    return;
  }
  displayChildren();
}
/*---------------------------------------------------------------------------------*/
void DrawableCompound::show( void )
{
  draw() ;
}
/*---------------------------------------------------------------------------------*/

}
