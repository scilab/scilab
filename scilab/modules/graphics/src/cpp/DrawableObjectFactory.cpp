/*------------------------------------------------------------------------*/
/* file: DrawableObjectFactory.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for drawable objects. Automaticaly create a drawable    */
/*        object with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------------------*/ 
void DrawableObjectFactory::setGraphicObj( sciPointObj * drawedObj )
{
  m_pDrawed = drawedObj ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObject * DrawableObjectFactory::create( void )
{
  DrawableObjectFactory * fact = NULL ;
  
  switch ( sciGetEntityType(m_pDrawed) )
    {
    case SCI_FIGURE:
      fact = new DrawableFigureFactory() ;
      break ;
    case SCI_SUBWIN:
      fact = new DrawableSubwinFactory() ;
      break;
    case SCI_TEXT:
      fact = new DrawableTextFactory() ;
      break;
    case SCI_POLYLINE:
      fact = new DrawablePolylineFactory() ;
      break;
    case SCI_LABEL:
      fact = new DrawableLabelFactory() ;
      break;
    case SCI_AGREG:
      fact = new DrawableCompoundFactory() ;
      break ;
    case SCI_SURFACE:
      fact = new DrawableSurfaceFactory() ;
      break ;
    case SCI_ARC:
      fact = new DrawableArcFactory() ;
      break ;
    case SCI_LEGEND:
      fact = new DrawableLegendFactory() ;
      break ;
    case SCI_AXES:
      fact = new DrawableAxesFactory() ;
      break ;
    case SCI_SEGS:
      fact = new DrawableSegsFactory() ;
      break ;
    case SCI_FEC:
      fact = new DrawableFecFactory() ;
      break ;
    case SCI_GRAYPLOT:
      fact = new DrawableGrayplotFactory() ;
      break ;
    case SCI_RECTANGLE: 
      fact = new DrawableRectangleFactory() ;
      break ;
    case SCI_TITLE:    
    case SCI_LIGHT:
    case SCI_PANNER:
    case SCI_SBH:
    case SCI_SBV:
    case SCI_MENU:
    case SCI_MENUCONTEXT:
    case SCI_STATUSB:
    case SCI_UIMENU: 
    case SCI_CONSOLE:
    case SCI_FRAME:
    case SCI_WINDOW:
    case SCI_WINDOWFRAME:
    case SCI_SCREEN:
    default:
      return NULL;
    }

  fact->setGraphicObj( m_pDrawed ) ;
  
  DrawableObject * drawer = fact->create() ;

  delete fact ;

  return drawer ;

}
/*------------------------------------------------------------------------------------------*/
