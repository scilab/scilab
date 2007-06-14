/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJoGL.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the marks on the corners of a rectangle        */
/*------------------------------------------------------------------------*/

#include "RectangleMarkDrawerJoGL.h"
#include "DrawableRectangle.h"

extern "C"
{
#include "GetProperty.h"
#include "JniUtils.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
RectangleMarkDrawerJoGL::RectangleMarkDrawerJoGL( DrawableRectangleImp * drawer )
  : DrawableObjectImp(drawer->getDrawer()), DrawRectangleStrategy( drawer ),
    DrawableObjectJoGL( drawer->getDrawer(), "org/scilab/modules/renderer/rectangleDrawing/RectangleMarkDrawerJoGL" )
{

}
/*------------------------------------------------------------------------------------------*/
void RectangleMarkDrawerJoGL::drawRectangle( void )
{

  sciPointObj * pObj = m_pDrawed->getDrawer()->getDrawedObject() ;
  initializeDrawing() ;

  // set the line parameters
  jniCallVoidFunctionSafe( m_oDrawableObject, "setMarkParameters", "IIIII",
                           sciGetGraphicContext(pObj)->markbackground, sciGetGraphicContext(pObj)->markforeground,
                           sciGetMarkSizeUnit(pObj), sciGetMarkSize(pObj), sciGetMarkStyle(pObj) ) ;

  // get the coordinates of the four corners of the rectangle.
  double corner1[3] ;
  double corner2[3] ;
  double corner3[3] ;
  double corner4[3] ;

  m_pDrawed->getDrawer()->getCornersCoordinates( corner1, corner2, corner3, corner4 ) ;

  // display the rectangle
  jniCallVoidFunctionSafe( m_oDrawableObject, "drawRectangle", "DDDDDDDDDDDD",
                           corner1[0], corner1[1], corner1[2],
                           corner2[0], corner2[1], corner2[2],
                           corner3[0], corner3[1], corner3[2],
                           corner4[0], corner4[1], corner4[2] ) ;

  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
}

