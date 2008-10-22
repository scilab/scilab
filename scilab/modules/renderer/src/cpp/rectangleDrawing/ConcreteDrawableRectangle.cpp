/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing driver independent algorithms to draw a
 * rectangle handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableRectangle.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

using namespace std;

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableRectangle::ConcreteDrawableRectangle( sciPointObj * pObj ) : DrawableRectangle(pObj)
{
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableRectangle::~ConcreteDrawableRectangle( void )
{
  removeDrawingStrategies();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableRectangle::addDrawingStrategy( DrawRectangleStrategy * strategy )
{
  m_oDrawingStrategies.push_back(strategy);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableRectangle::removeDrawingStrategies( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableRectangle::getCornersCoordinates( double corner1[3], double corner2[3], double corner3[3], double corner4[3] )
{

  //    <--- w --->
  // (x,y)--------c4
  //   |           |
  // h |           |
  //   |           |
  //   c2---------c3

  sciRectangle * ppRect = pRECTANGLE_FEATURE(m_pDrawed) ;
  corner1[0] = ppRect->x ;
  corner1[1] = ppRect->y ;
  corner1[2] = ppRect->z ;

  corner2[0] = ppRect->x ;
  corner2[1] = ppRect->y - ppRect->height ;
  corner2[2] = ppRect->z ;

  corner3[0] = ppRect->x + ppRect->width  ;
  corner3[1] = ppRect->y - ppRect->height ;
  corner3[2] = ppRect->z ;

  corner4[0] = ppRect->x + ppRect->width ;
  corner4[1] = ppRect->y ;
  corner4[2] = ppRect->z ;

  // apply log scale if needed
  pointScale(corner1[0], corner1[1], corner1[2], &(corner1[0]), &(corner1[1]), &(corner1[2]));
  pointScale(corner2[0], corner2[1], corner2[2], &(corner2[0]), &(corner2[1]), &(corner2[2]));
  pointScale(corner3[0], corner3[1], corner3[2], &(corner3[0]), &(corner3[1]), &(corner3[2]));
  pointScale(corner4[0], corner4[1], corner4[2], &(corner4[0]), &(corner4[1]), &(corner4[2]));

}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableRectangle::drawRectangle(void)
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->drawRectangle();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableRectangle::redrawRectangle(void)
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->redrawRectangle();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableRectangle::showRectangle(void)
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->show();
  }
}
/*---------------------------------------------------------------------------------*/
}
