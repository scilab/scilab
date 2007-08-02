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
  : DrawableClippedObjectJoGL(drawer)
{

}
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJoGL::~DrawableRectangleJoGL( void )
{

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
DrawableRectangle * DrawableRectangleJoGL::getRectangleDrawer( void )
{
  return dynamic_cast<DrawableRectangle *>(getDrawer()) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJavaMapper * DrawableRectangleJoGL::getRectangleJavaMapper(void)
{
  return dynamic_cast<DrawableRectangleJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::destroyRectangle( void )
{
  DrawableObjectJoGL::destroy() ;
}
/*------------------------------------------------------------------------------------------*/

}
