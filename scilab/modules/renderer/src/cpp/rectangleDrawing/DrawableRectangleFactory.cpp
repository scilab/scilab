/*------------------------------------------------------------------------*/
/* file: DrawableRectangleFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for rectangle objects. Automaticaly create a rectangle  */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableRectangleFactory.h"
#include "DrawableRectangleBridgeFactory.h"
#include "ConcreteDrawableRectangle.h"
#include "../getHandleDrawer.h"
#include "../subwinDrawing/CameraFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableRectangleFactory::create( void )
{
  ConcreteDrawableRectangle * newRect = new ConcreteDrawableRectangle( m_pDrawed ) ;
  DrawableRectangleBridgeFactory imp ;
  imp.setDrawedRectangle( newRect ) ;
  newRect->setDrawableImp( imp.create() ) ;

  return newRect ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleFactory::update( void )
{
  DrawableRectangleBridgeFactory imp ;
  imp.setDrawedRectangle( getRectangleDrawer(m_pDrawed) ) ;
  imp.update();
}
/*---------------------------------------------------------------------------------*/

}
