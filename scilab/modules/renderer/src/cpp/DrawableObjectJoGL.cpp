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
#include "JniUtils.h"
#include "GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableObjectJoGL::DrawableObjectJoGL( DrawableObject * drawer, const char * className ) : DrawableObjectImp(drawer)
{
  m_oDrawableClass  = NULL ;
  m_oDrawableObject = NULL ;
  jniCreateDefaultInstanceSafe( className, &m_oDrawableClass, &m_oDrawableObject ) ;
  jniCallVoidFunctionSafe( m_oDrawableObject, "setFigureIndex", "I", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObjectJoGL::~DrawableObjectJoGL( void )
{
  destroy() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeDrawing( void )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "initializeDrawing", "I", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endDrawing( void )
{   
  jniCallVoidFunctionSafe( m_oDrawableObject, "endDrawing", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeShowing( void )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "initializeShowing", "I", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endShowing( void )
{   
  jniCallVoidFunctionSafe( m_oDrawableObject, "endShowing", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::show( void )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "show", "I", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::destroy( void )
{
  if ( m_oDrawableObject != NULL )
  {
    jniCallVoidFunctionSafe( m_oDrawableObject, "destroy", "" ) ;
    jniDeleteLocalEntity( m_oDrawableObject ) ;
  }
  m_oDrawableObject = NULL ;
  m_oDrawableClass  = NULL ;
}
/*------------------------------------------------------------------------------------------*/
}
