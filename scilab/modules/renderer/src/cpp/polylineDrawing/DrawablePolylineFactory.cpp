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
#include "PolylineMarkDrawerJoGL.hxx"
#include "PolylineArrowDrawerJoGL.hxx"
#include "PolylineBarDrawerJoGL.hxx"
#include "PolylineInterpColorDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawablePolylineFactory::create( void )
{
  ConcreteDrawablePolyline * newPoly = new ConcreteDrawablePolyline( m_pDrawed ) ;
  DrawablePolylineBridgeFactory imp ;
  imp.setDrawedPolyline( newPoly ) ;
  newPoly->setDrawableImp( imp.create() ) ;
  setStrategies(newPoly);

  return newPoly ;
}
/*---------------------------------------------------------------------------------*/
void DrawablePolylineFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawablePolyline *>(getPolylineDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawablePolylineFactory::setStrategies( ConcreteDrawablePolyline * polyline )
{
  sciPointObj * pPolyline = polyline->getDrawedObject();

  polyline->removeDecompositionStrategy();
  polyline->removeDrawingStrategies();

  if (sciGetPolylineStyle(pPolyline) == 5)
  {
    polyline->addDrawingStrategy(new PolylineFillDrawerJoGL(polyline));
  }
  else if (sciGetIsFilled(pPolyline))
  {
    if (sciGetIsColorInterpolated(pPolyline) && sciGetPolylineStyle(pPolyline) != 2)
    {
      polyline->addDrawingStrategy(new PolylineInterpColorDrawerJoGL(polyline));
    }
    else
    {
      polyline->addDrawingStrategy(new PolylineFillDrawerJoGL(polyline));
    }
  }

  if (sciGetPolylineStyle(pPolyline) == 2)
  {
    polyline->setDecompositionStrategy(new StairCaseDecomposition(polyline));
  }
  else
  {
    polyline->setDecompositionStrategy(new InterpolatedDecomposition(polyline));
  }

  if (sciGetPolylineStyle(pPolyline) == 4)
  {
    polyline->addDrawingStrategy(new PolylineArrowDrawerJoGL(polyline));
  }

  if (sciGetPolylineStyle(pPolyline) == 3 || sciGetPolylineStyle(pPolyline) == 6)
  {
    polyline->addDrawingStrategy(new PolylineBarDrawerJoGL(polyline));
  }

  if (sciGetIsLine(pPolyline))
  {
    polyline->addDrawingStrategy(new PolylineLineDrawerJoGL(polyline));
  }

  if (sciGetIsMark(pPolyline))
  {
    polyline->addDrawingStrategy(new PolylineMarkDrawerJoGL(polyline));
  }
  
}
/*---------------------------------------------------------------------------------*/

}
