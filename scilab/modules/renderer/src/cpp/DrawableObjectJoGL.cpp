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
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "setFigureIndex", "(I)V", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObjectJoGL::~DrawableObjectJoGL( void )
{
  destroy() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeDrawing( void )
{
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "initializeDrawing", "(I)V", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endDrawing( void )
{   
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "endDrawing", "()V" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeShowing( void )
{
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "initializeShowing", "(I)V", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endShowing( void )
{   
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "endShowing", "()V" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::show( void )
{
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "show", "(I)V", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::destroy( void )
{
  if ( m_oDrawableObject != NULL )
  {
    jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "destroy", "(I)V", sciGetNum(sciGetParentFigure(m_pDrawer->getDrawedObject())) ) ;
    jniDeleteGlobalEntity(m_oDrawableObject) ;
    jniDeleteGlobalEntity(m_oDrawableClass) ;
  }
  m_oDrawableObject = NULL ;
  m_oDrawableClass  = NULL ;
}
/*------------------------------------------------------------------------------------------*/
}
