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
#include "ConcreteDrawablePolyline.hxx"
#include "getHandleDrawer.h"
#include "PolylineLineDrawerJoGL.hxx"
#include "PolylineFillDrawerJoGL.hxx"
#include "InterpolatedDecomposition.hxx"
#include "StairCaseDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawablePolylineFactory::create( void )
{
  DrawablePolyline * newPoly = new ConcreteDrawablePolyline( m_pDrawed ) ;
  DrawablePolylineBridgeFactory imp ;
  imp.setDrawedPolyline( newPoly ) ;
  newPoly->setDrawableImp( imp.create() ) ;

  return newPoly ;
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineFactory::update( void )
{
  setStrategies(getPolylineDrawer(m_pDrawed));
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineFactory::setStrategies( DrawablePolyline * polyline )
{
  ConcreteDrawablePolyline * cPolyline = dynamic_cast<ConcreteDrawablePolyline *>(polyline);
  sciPointObj * pPolyline = polyline->getDrawedObject();

  cPolyline->removeDecompositionStrategy();
  cPolyline->removeDrawingStrategies();

  if (sciGetIsLine(pPolyline))
  {
    cPolyline->addDrawingStrategy(new PolylineLineDrawerJoGL(polyline));
  }

  if (sciGetIsFilled(pPolyline) || sciGetPolylineStyle(pPolyline) == 5)
  {
    cPolyline->addDrawingStrategy(new PolylineFillDrawerJoGL(polyline));
  }
  
  if (sciGetPolylineStyle(pPolyline) == 2)
  {
    cPolyline->setDecompositionStrategy(new StairCaseDecomposition(polyline));
  }
  else
  {
    cPolyline->setDecompositionStrategy(new InterpolatedDecomposition(polyline));
  }
  
}
/*------------------------------------------------------------------------------------------*/

}
