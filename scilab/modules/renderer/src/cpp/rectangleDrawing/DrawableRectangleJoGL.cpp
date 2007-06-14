/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object with JoGL                                      */
/*------------------------------------------------------------------------*/

#include <list>
#include "../DrawableObjectJoGL.h"
#include "DrawableRectangle.h"
#include "DrawableRectangleJoGL.h"

extern "C"
{
#include "JniUtils.h"
}


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJoGL::DrawableRectangleJoGL( DrawableRectangle * drawer )
  : DrawableObjectImp(drawer), DrawableRectangleImp(drawer), DrawableObjectJoGL(drawer, "org/scilab/modules/renderer/rectangleDrawing/DrawableRectangleJoGL")
{

}
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJoGL::~DrawableRectangleJoGL( void )
{
  destroyRectangle() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::drawRectangle( void )
{
  DrawableRectangleImp::drawRectangle();
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::show( void )
{
  std::list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->show() ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::destroyRectangle( void )
{
  DrawableObjectJoGL::destroy() ;
}
/*------------------------------------------------------------------------------------------*/
}
