/*------------------------------------------------------------------------*/
/* file: DrawableRectangleFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for rectangle objects. Automaticaly create a rectangle  */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableRectangleFactory.h"
#include "DrawableRectangleImpFactory.h"
#include "DrawableRectangle.h"
#include "../getHandleDrawer.h"
#include "../subwinDrawing/CameraFactory.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableRectangleFactory::create( void )
{
  DrawableRectangle * newRect = new DrawableRectangle( m_pDrawed ) ;
  DrawableRectangleImpFactory imp ;
  imp.setDrawedRectangle( newRect ) ;
  newRect->setDrawableImp( imp.create() ) ;

  return newRect ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleFactory::update( void )
{
  DrawableRectangleImpFactory imp ;
  imp.setDrawedRectangle( getRectangleDrawer(m_pDrawed) ) ;
  imp.update();
}
/*------------------------------------------------------------------------------------------*/

}
