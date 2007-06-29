/*------------------------------------------------------------------------*/
/* file: DrawablePolylineFactory.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for polyline objects. Automaticaly create a polyline    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineFactory.h"
#include "DrawablePolyline.h"
#include "DrawablePolylineBridgeFactory.h"

#include "../subwinDrawing/DrawableSubwinFactory.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawablePolylineFactory::create( void )
{
  /*DrawablePolyline * newPoly = new DrawablePolyline( m_pDrawed ) ;
  DrawablePolylineBridgeFactory imp ;
  imp.setDrawedPolyline( newPoly ) ;
  newPoly->setDrawableImp( imp.create() ) ;

  return newPoly ;*/
  DrawableSubwinFactory fact ;
  fact.setGraphicObj(m_pDrawed) ;
  return fact.create();
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineFactory::update( void )
{
  // nothing for now
}
/*------------------------------------------------------------------------------------------*/

}
