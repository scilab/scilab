/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a rectangle object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "DrawableRectangleBridge.h"
#include "DrawableRectangle.h"

namespace sciGraphics
{

using namespace std ;

/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge::DrawableRectangleBridge( void )
{
  m_oDrawingStrategies.clear() ;
}
/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge::~DrawableRectangleBridge( void )
{
  // delete the strategies
  removeDrawingStrategies() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridge::drawRectangle( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->drawRectangle() ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridge::addDrawingStrategy( DrawRectangleStrategy * strategy )
{
  m_oDrawingStrategies.push_back( strategy ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridge::removeDrawingStrategies( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    delete *it ;
    *it = NULL ;
  }
  m_oDrawingStrategies.clear() ;
}
/*---------------------------------------------------------------------------------*/
}
