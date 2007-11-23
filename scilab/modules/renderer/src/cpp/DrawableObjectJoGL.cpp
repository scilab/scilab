/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Objects which can be drawn using JoGL                           */
/*------------------------------------------------------------------------*/

#include "DrawableObjectJoGL.h"
#include "DrawableObject.h"
extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableObjectJoGL::DrawableObjectJoGL( DrawableObject * drawer )
{
  m_pDrawer = drawer ;
  m_pJavaMapper = NULL;
}
/*---------------------------------------------------------------------------------*/
DrawableObjectJoGL::~DrawableObjectJoGL( void )
{
  destroy() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeDrawing( void )
{
  getJavaMapper()->setFigureIndex(sciGetNum(sciGetParentFigure(getDrawer()->getDrawedObject())));
  getJavaMapper()->initializeDrawing(sciGetNum(sciGetParentFigure(getDrawer()->getDrawedObject())) );
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endDrawing( void )
{
  getJavaMapper()->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeShowing( void )
{
  
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endShowing( void )
{   
  
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::show( void )
{
  getJavaMapper()->show(sciGetNum(sciGetParentFigure(getDrawer()->getDrawedObject())));
}
/*---------------------------------------------------------------------------------*/
void DrawableObjectJoGL::destroy( void )
{
  if (m_pJavaMapper != NULL)
  {
    getJavaMapper()->destroy(sciGetNum(sciGetParentFigure(getDrawer()->getDrawedObject())));
    delete getJavaMapper();
    setJavaMapper(NULL);
  }
}
/*---------------------------------------------------------------------------------*/
}
