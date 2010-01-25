/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for polyline objects. Automatically create a polyline
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
#include "BarDecomposition.hxx"

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

  polyline->removeDrawingStrategies();


  switch(sciGetPolylineStyle(pPolyline))
  {
  case 1:
    polyline->setDecompositionStrategy(new InterpolatedDecomposition(polyline));
    break;
  case 2:
    // stair case polyline
    polyline->setDecompositionStrategy(new StairCaseDecomposition(polyline));
    break;
  case 3:
  case 6:
  case 7:
    // bar polyline
    polyline->setDecompositionStrategy(new BarDecomposition(polyline));
    polyline->addDrawingStrategy(new PolylineBarDrawerJoGL(polyline));
    break;
  case 4:
    // arrow drawing
    polyline->addDrawingStrategy(new PolylineArrowDrawerJoGL(polyline));
    polyline->setDecompositionStrategy(new InterpolatedDecomposition(polyline));
    break;
  case 5:
    // fill drawing
    polyline->addDrawingStrategy(new PolylineFillDrawerJoGL(polyline));
    polyline->setDecompositionStrategy(new InterpolatedDecomposition(polyline));
    break;
  default:
    polyline->setDecompositionStrategy(new InterpolatedDecomposition(polyline));
    break;
  }

  // if polyline style is 5, fill mode has already been enable
  if (sciGetIsFilled(pPolyline) && sciGetPolylineStyle(pPolyline) != 5)
  {
    // interp vector does not work with staircase since it create a polygon with
    // more than 4 vertices.
    if (sciGetIsColorInterpolated(pPolyline) && sciGetPolylineStyle(pPolyline) != 2)
    {
      polyline->addDrawingStrategy(new PolylineInterpColorDrawerJoGL(polyline));
    }
    else
    {
      polyline->addDrawingStrategy(new PolylineFillDrawerJoGL(polyline));
    }
  }

  // lines are drawn after filling to be drawn above in 2D
	if (sciGetIsDisplayingLines(pPolyline))
  {
    polyline->addDrawingStrategy(new PolylineLineDrawerJoGL(polyline));
  }

  // set mark drawin at the end so they will be drawn in front of lines and fill
  // in 2D
  if (sciGetIsMark(pPolyline))
  {
    polyline->addDrawingStrategy(new PolylineMarkDrawerJoGL(polyline));
  }
  
}
/*---------------------------------------------------------------------------------*/

}
