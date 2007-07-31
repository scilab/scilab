/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJoGL.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the inside of a rectangle                      */
/*------------------------------------------------------------------------*/

#include "RectangleFillDrawerJoGL.h"
#include "DrawableRectangle.h"
extern "C"
{
#include "GetProperty.h"
#include "JniUtils.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
RectangleFillDrawerJoGL::RectangleFillDrawerJoGL( DrawableRectangleBridge * drawer )
  : DrawRectangleStrategy( drawer ),
    DrawableObjectJoGL(drawer->getRectangleDrawer(),"org/scilab/modules/renderer/rectangleDrawing/RectangleFillDrawerJoGL" )
{

}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJoGL::drawRectangle( void )
{
  sciPointObj * pObj = m_pDrawed->getRectangleDrawer()->getDrawedObject() ;
  initializeDrawing() ;

  // set the line parameters
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "setBackColor", "(I)V", sciGetGraphicContext(pObj)->backgroundcolor ) ;

  // get the coordinates of the four corners of the rectangle.
  double corner1[3] ;
  double corner2[3] ;
  double corner3[3] ;
  double corner4[3] ;

  m_pDrawed->getRectangleDrawer()->getCornersCoordinates( corner1, corner2, corner3, corner4 ) ;

  // display the rectangle
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "drawRectangle", "(DDDDDDDDDDDD)V",
                             corner1[0], corner1[1], corner1[2],
                             corner2[0], corner2[1], corner2[2],
                             corner3[0], corner3[1], corner3[2],
                             corner4[0], corner4[1], corner4[2] ) ;

  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
}
