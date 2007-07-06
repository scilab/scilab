/*------------------------------------------------------------------------*/
/* file: DrawablePolylineBridge.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        polyline object                                                 */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineBridge.h"
#include "DrawablePolyline.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawablePolylineBridge::DrawablePolylineBridge( DrawablePolyline * drawer ) : DrawableObjectImp( drawer )
{

}
/*------------------------------------------------------------------------------------------*/
DrawablePolylineBridge::~DrawablePolylineBridge( void )
{

}
/*------------------------------------------------------------------------------------------*/
DrawablePolyline * DrawablePolylineBridge::getDrawer( void )
{
  return dynamic_cast<DrawablePolyline *>(m_pDrawer) ;
}
/*------------------------------------------------------------------------------------------*/

}
