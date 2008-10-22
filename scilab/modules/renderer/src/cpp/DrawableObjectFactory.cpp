/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for drawable objects. Automaticaly create a drawable    
 * object with the right algorithms from the garphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "GetProperty.h"
}

#include "DrawableObjectFactory.h"
#include "figureDrawing/DrawableFigureFactory.h"
#include "subwinDrawing/DrawableSubwinFactory.h"
#include "textDrawing/DrawableTextFactory.h"
#include "polylineDrawing/DrawablePolylineFactory.h"
#include "labelDrawing/DrawableLabelFactory.h"
#include "compoundDrawing/DrawableCompoundFactory.h"
#include "surfaceDrawing/DrawableSurfaceFactory.h"
#include "arcDrawing/DrawableArcFactory.h"
#include "axesDrawing/DrawableAxesFactory.h"
#include "fecDrawing/DrawableFecFactory.h"
#include "grayplotDrawing/DrawableGrayplotFactory.h"
#include "legendDrawing/DrawableLegendFactory.h"
#include "rectangleDrawing/DrawableRectangleFactory.h"
#include "segsDrawing/DrawableSegsFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/ 
void DrawableObjectFactory::setGraphicObj( sciPointObj * drawedObj )
{
  m_pDrawed = drawedObj ;
}
/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableObjectFactory::create( void )
{
  DrawableObjectFactory * fact = createRightFactory() ;
  
  fact->setGraphicObj( m_pDrawed ) ;
  
  DrawableObject * drawer = fact->create() ;

  delete fact ;

  return drawer ;

}
/*---------------------------------------------------------------------------------*/
void DrawableObjectFactory::update( void )
{
  DrawableObjectFactory * fact = createRightFactory() ;

  fact->setGraphicObj( m_pDrawed ) ;

  fact->update() ;

  delete fact ;

}
/*---------------------------------------------------------------------------------*/
DrawableObjectFactory * DrawableObjectFactory::createRightFactory( void )
{
  switch ( sciGetEntityType(m_pDrawed) )
  {
  case SCI_FIGURE:
    return new DrawableFigureFactory() ;
    break ;
  case SCI_SUBWIN:
    return new DrawableSubwinFactory() ;
    break;
  case SCI_TEXT:
    return new DrawableTextFactory() ;
    break;
  case SCI_POLYLINE:
    return new DrawablePolylineFactory() ;
    break;
  case SCI_LABEL:
    return new DrawableLabelFactory() ;
    break;
  case SCI_AGREG:
    return new DrawableCompoundFactory() ;
    break ;
  case SCI_SURFACE:
    return new DrawableSurfaceFactory() ;
    break ;
  case SCI_ARC:
    return new DrawableArcFactory() ;
    break ;
  case SCI_LEGEND:
    return new DrawableLegendFactory() ;
    break ;
  case SCI_AXES:
    return new DrawableAxesFactory() ;
    break ;
  case SCI_SEGS:
    return new DrawableSegsFactory() ;
    break ;
  case SCI_FEC:
    return new DrawableFecFactory() ;
    break ;
  case SCI_GRAYPLOT:
    return new DrawableGrayplotFactory() ;
    break ;
  case SCI_RECTANGLE: 
    return new DrawableRectangleFactory() ;
    break ;
  case SCI_UIMENU:
  default:
    return NULL;
  }
}
/*---------------------------------------------------------------------------------*/
}
