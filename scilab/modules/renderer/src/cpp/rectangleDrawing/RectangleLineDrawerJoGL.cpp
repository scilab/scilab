/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing the outline of a rectangle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "RectangleLineDrawerJoGL.h"
#include "DrawableObjectJoGL.h"
#include "RectangleLineDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
RectangleLineDrawerJoGL::RectangleLineDrawerJoGL( DrawableRectangle * drawer )
  : DrawRectangleStrategy( drawer ), DrawableObjectJoGL(drawer)
{
  setJavaMapper(new RectangleLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
RectangleLineDrawerJoGL::~RectangleLineDrawerJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJoGL::drawRectangle( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject() ;
  initializeDrawing() ;

  // set the line parameters
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pObj)->foregroundcolor,
                                               (float)sciGetLineWidth(pObj),
                                               sciGetLineStyle(pObj));

  // get the coordinates of the four corners of the rectangle.
  double corner1[3] ;
  double corner2[3] ;
  double corner3[3] ;
  double corner4[3] ;
  
  m_pDrawed->getCornersCoordinates( corner1, corner2, corner3, corner4 ) ;

  // display the rectangle
  getLineDrawerJavaMapper()->drawRectangle(corner1[0], corner1[1], corner1[2],
                                           corner2[0], corner2[1], corner2[2],
                                           corner3[0], corner3[1], corner3[2],
                                           corner4[0], corner4[1], corner4[2]);

  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
RectangleLineDrawerJavaMapper * RectangleLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<RectangleLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
