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
  m_bNeedDraw = true   ; // a first call to draw is necessary
  m_bNeedRedraw = false  ;
  m_pImp = NULL ;
  reinitMove();
}
/*---------------------------------------------------------------------------------*/
DrawableObject::~DrawableObject( void )
{
  setDrawableImp(NULL);
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::display( void )
{
  if ( m_bNeedDraw )
  {
    if ( draw() == SUCCESS)
    {
      m_bNeedDraw = false;
      m_bNeedRedraw = false ;
    } 
  }
  else if (m_bNeedRedraw)
  {
    if (redraw() == SUCCESS)
    {
      m_bNeedRedraw = false;
    }
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
  m_bNeedDraw = true ;
}
/*---------------------------------------------------------------------------------*/
void DrawableObject::familyHasChanged( void )
{
  // force redraw
  hasChanged();

  // do the same for childrens
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
void DrawableObject::parentSubwinChanged( void )
{
  // just call the function on children
  m_bNeedRedraw = true;
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    if (   sciGetEntityType(curSon->pointobj) != SCI_UICONTROL
        && sciGetEntityType(curSon->pointobj) != SCI_UIMENU)
    {
      getHandleDrawer( curSon->pointobj )->parentSubwinChanged();
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
    sciEntityType curType = sciGetEntityType(curSon->pointobj);
    // uicontrol and menus are directly drawn by Java
    // while text and labels are drawn at the end
    if (   curType != SCI_UICONTROL
        && curType != SCI_UIMENU
        && curType != SCI_TEXT
        && curType != SCI_LABEL)
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
void DrawableObject::move(const double translation[3])
{
  // disable move in logarithmic scale
  // move is to be used with affine transformations
  // TODO create specific cases for each object
  sciPointObj * parentSubwin = sciGetParentSubwin(m_pDrawed);
  char logFlags[3];
  sciGetLogFlags(parentSubwin, logFlags);
  if (logFlags[0] == 'l' || logFlags[1] == 'l' || logFlags[2] == 'l')
  {
    // use has changed instead
    hasChanged();
    return;
  }

  // add the new translation
  m_aMoveTranslation[0] += translation[0];
  m_aMoveTranslation[1] += translation[1];
  m_aMoveTranslation[2] += translation[2];
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::reinitMove(void)
{

  m_aMoveTranslation[0] = 0.0;
  m_aMoveTranslation[1] = 0.0;
  m_aMoveTranslation[2] = 0.0;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::getMoveTranslation(double translation[3])
{
  translation[0] = m_aMoveTranslation[0];
  translation[1] = m_aMoveTranslation[1];
  translation[2] = m_aMoveTranslation[2];
}
/*------------------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableObject::redraw(void)
{
  return show();
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::translate(void)
{
  // translate only if needed
  if (m_aMoveTranslation[0] != 0.0 ||
      m_aMoveTranslation[1] != 0.0 ||
      m_aMoveTranslation[2] != 0.0)
  {
    getDrawableImp()->translate(m_aMoveTranslation);
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::endTranslate(void)
{
  if (m_aMoveTranslation[0] != 0.0 ||
    m_aMoveTranslation[1] != 0.0 ||
    m_aMoveTranslation[2] != 0.0)
  {
    getDrawableImp()->endTranslate();
  }
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
