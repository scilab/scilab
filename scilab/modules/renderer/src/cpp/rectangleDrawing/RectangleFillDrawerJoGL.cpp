/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing the inside of a rectangle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "RectangleFillDrawerJoGL.h"
#include "DrawableRectangle.h"
extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
RectangleFillDrawerJoGL::RectangleFillDrawerJoGL( DrawableRectangle * drawer )
  : DrawRectangleStrategy( drawer ), DrawableObjectJoGL(drawer)
{
  setJavaMapper(new RectangleFillDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
RectangleFillDrawerJoGL::~RectangleFillDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void RectangleFillDrawerJoGL::drawRectangle( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject() ;
  initializeDrawing() ;

  // set the line parameters
  getFillDrawerJavaMapper()->setBackColor(sciGetGraphicContext(pObj)->backgroundcolor) ;

  // get the coordinates of the four corners of the rectangle.
  double corner1[3] ;
  double corner2[3] ;
  double corner3[3] ;
  double corner4[3] ;

  m_pDrawed->getCornersCoordinates( corner1, corner2, corner3, corner4 ) ;

  // display the rectangle
  getFillDrawerJavaMapper()->drawRectangle(corner1[0], corner1[1], corner1[2],
                                           corner2[0], corner2[1], corner2[2],
                                           corner3[0], corner3[1], corner3[2],
                                           corner4[0], corner4[1], corner4[2]);


  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
RectangleFillDrawerJavaMapper * RectangleFillDrawerJoGL::getFillDrawerJavaMapper(void)
{
  return dynamic_cast<RectangleFillDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
