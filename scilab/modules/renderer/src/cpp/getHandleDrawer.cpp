/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains the function to retrieve a the drawer of a graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "getHandleDrawer.h"
#include "DrawableObjectFactory.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * getHandleDrawer( sciPointObj * pObj )
{
  if ( pObj->pDrawer != NULL )
  {
    return getHandleDrawerPointer( pObj ) ;
  }
  
  /* We need to create the drawer */

  DrawableObjectFactory creator ;
  creator.setGraphicObj( pObj ) ;

  DrawableObject * drawer = creator.create() ;

  setHandleDrawerPointer( pObj, drawer ) ;

  return drawer ;
}
/*---------------------------------------------------------------------------------*/
DrawableObject * getHandleDrawerPointer( sciPointObj * pObj )
{
  return ((DrawableObjectWrapper *)(pObj->pDrawer))->drawer ;
}
/*---------------------------------------------------------------------------------*/
void setHandleDrawerPointer( sciPointObj * pObj, DrawableObject * drawer )
{
  if ( pObj->pDrawer == NULL )
  {
    pObj->pDrawer = new DrawableObjectWrapper() ;
  }
  ((DrawableObjectWrapper *)(pObj->pDrawer))->drawer = drawer ;
}
/*---------------------------------------------------------------------------------*/
DrawableFigure * getFigureDrawer( sciPointObj * pFigure )
{
  return dynamic_cast<DrawableFigure *>(getHandleDrawer(pFigure));
}
/*---------------------------------------------------------------------------------*/
DrawableRectangle * getRectangleDrawer( sciPointObj * pRectangle )
{
  return dynamic_cast<DrawableRectangle *>(getHandleDrawer(pRectangle));
}
/*---------------------------------------------------------------------------------*/
DrawableArc * getArcDrawer( sciPointObj * pArc )
{
  return dynamic_cast<DrawableArc *>(getHandleDrawer(pArc));
}
/*---------------------------------------------------------------------------------*/
DrawablePolyline * getPolylineDrawer( sciPointObj * pPolyline )
{
  return dynamic_cast<DrawablePolyline *>(getHandleDrawer(pPolyline));
}
/*---------------------------------------------------------------------------------*/
DrawableText * getTextDrawer( sciPointObj * pText )
{
  return dynamic_cast<DrawableText *>(getHandleDrawer(pText));
}
/*---------------------------------------------------------------------------------*/
DrawableSubwin * getSubwinDrawer( sciPointObj * pSubwin )
{
  return dynamic_cast<DrawableSubwin *>(getHandleDrawer(pSubwin));
}
/*---------------------------------------------------------------------------------*/
DrawableLabel * getLabelDrawer( sciPointObj * pLabel )
{
  return dynamic_cast<DrawableLabel *>(getHandleDrawer(pLabel));
}
/*---------------------------------------------------------------------------------*/
DrawableSurface * getSurfaceDrawer( sciPointObj * pSurface )
{
  return dynamic_cast<DrawableSurface *>(getHandleDrawer(pSurface));
}
/*---------------------------------------------------------------------------------*/
DrawableSegs * getSegsDrawer( sciPointObj * pSegs )
{
  return dynamic_cast<DrawableSegs *>(getHandleDrawer(pSegs));
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplot * getGrayplotDrawer( sciPointObj * pGrayplot )
{
  return dynamic_cast<DrawableGrayplot *>(getHandleDrawer(pGrayplot));
}
/*---------------------------------------------------------------------------------*/
DrawableFec * getFecDrawer( sciPointObj * pFec )
{
  return dynamic_cast<DrawableFec *>(getHandleDrawer(pFec));
}
/*---------------------------------------------------------------------------------*/
DrawableAxes * getAxesDrawer( sciPointObj * pAxes )
{
  return dynamic_cast<DrawableAxes *>(getHandleDrawer(pAxes));
}
/*---------------------------------------------------------------------------------*/
}
