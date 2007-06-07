/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object with JoGL                                      */
/*------------------------------------------------------------------------*/

#include "../DrawableObjectJoGL.h"
#include "DrawableRectangle.h"
#include "DrawableRectangleJoGL.h"
extern "C"
{
#include "JniUtils.h"
}

using namespace std ;

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJoGL::DrawableRectangleJoGL( DrawableRectangle * drawer )
  : DrawableObjectImp(drawer), DrawableRectangleImp(drawer), DrawableObjectJoGL(drawer, "org/scilab/modules/graphics/figureDrawing/DrawableRectangleJoGL")
{
  m_oDrawingStrategies.clear() ;
}
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJoGL::~DrawableRectangleJoGL( void )
{
  destroyRectangle() ;

  // delete the strategies
  list<DrawRectangleStrategyJoGL *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    delete *it ;
    *it = NULL ;
  }
  m_oDrawingStrategies.clear() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::drawRectangle( void )
{
  list<DrawRectangleStrategyJoGL *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->drawRectangle() ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::addDrawingStrategy( DrawRectangleStrategyJoGL * strategy )
{
  m_oDrawingStrategies.push_back( strategy ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJoGL::destroyRectangle( void )
{
  DrawableObjectJoGL::destroy() ;
}
/*------------------------------------------------------------------------------------------*/
}
