/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class dedicated for the graphic rendering of graphic handles 
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

#include "DrawableObject.h"
#include "getHandleDrawer.h"
#include "DrawableObjectFactory.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject::DrawableObject( sciPointObj * drawed )
{
  m_pDrawed     = drawed ;
  m_bNeedRedraw = true   ; // a first call to draw is necessary
  m_pImp = NULL ;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::~DrawableObject( void )
{
  setDrawableImp(NULL);
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::display( void )
{
  if ( m_bNeedRedraw )
  {
    draw() ;
    m_bNeedRedraw = false ;
  }
  else
  {
    show() ;
  }
  
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::hasChanged( void )
{
  DrawableObjectFactory updater ;
  updater.setGraphicObj( m_pDrawed ) ;
  updater.update() ;
  m_bNeedRedraw = true ;
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::familyHasChanged( void )
{
  hasChanged();
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    if (sciGetEntityType(curSon->pointobj) != SCI_UICONTROL
        && sciGetEntityType(curSon->pointobj) != SCI_UIMENU) 
      {
        getHandleDrawer( curSon->pointobj )->familyHasChanged();
      }
    curSon = curSon->pprev ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::displayChildren( void )
{
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    if (sciGetEntityType(curSon->pointobj) != SCI_UICONTROL
        && sciGetEntityType(curSon->pointobj) != SCI_UIMENU)
      {
        getHandleDrawer( curSon->pointobj )->display() ;
      }
    curSon = curSon->pprev ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::pointScale(double xCoord, double yCoord, double zCoord,
                                double * xScaled, double * yScaled, double * zScaled)
{
  getSubwinDrawer(sciGetParentSubwin(m_pDrawed))->pointScale(xCoord, yCoord, zCoord,
                                                             xScaled, yScaled, zScaled);
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::inversePointScale(double xScaled, double yScaled, double zScaled,
                                       double * xCoord, double * yCoord, double * zCoord)
{
  getSubwinDrawer(sciGetParentSubwin(m_pDrawed))->inversePointScale(xScaled, yScaled, zScaled,
                                                                    xCoord, yCoord, zCoord);
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::pointScale(double vectorX[], double vectorY[], double vectorZ[], int vectorLength)
{
  getSubwinDrawer(sciGetParentSubwin(m_pDrawed))->pointScale(vectorX, vectorY, vectorZ, vectorLength);
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::directionScale(double xCoord, double yCoord, double zCoord,
                                    double startingPointX, double startingPointY, double startingPointZ,
                                    double * xScaled, double * yScaled, double * zScaled)
{
  getSubwinDrawer(sciGetParentSubwin(m_pDrawed))->directionScale(xCoord, yCoord, zCoord, startingPointX,
                                                                 startingPointY, startingPointZ,
                                                                 xScaled, yScaled, zScaled);
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::directionScale(double vectorX[], double vectorY[], double vectorZ[],
                                    double startingPointsX[], double startingPointsY[],
                                    double startingPointsZ[], double vectorLength)
{
  getSubwinDrawer(sciGetParentSubwin(m_pDrawed))->directionScale(vectorX, vectorY, vectorZ,
                                                                 startingPointsX, startingPointsY,
                                                                 startingPointsZ, vectorLength);
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::setDrawableImp( DrawableObjectBridge * imp )
{
  if (m_pImp != NULL)
  {
    delete m_pImp;
  }
  m_pImp = imp;
}
/*------------------------------------------------------------------------------------------*/
bool DrawableObject::checkVisibility( void )
{
  return sciGetVisibility( m_pDrawed ) == TRUE ;
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::initializeDrawing( void )
{
  getDrawableImp()->initializeDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::endDrawing( void )
{
  getDrawableImp()->endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
}
