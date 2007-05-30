/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Objects which can be drawn using JoGL                           */
/*------------------------------------------------------------------------*/

#include "DrawableObjectJoGL.h"
extern "C"
{
#include "JniUtils.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableObjectJoGL::DrawableObjectJoGL( const char * className ) : DrawableObjectImp()
{
  m_oDrawableClass  = NULL ;
  m_oDrawableObject = NULL ;
  jniCreateDefaultInstanceSafe( className, &m_oDrawableClass, &m_oDrawableObject ) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObjectJoGL::DrawableObjectJoGL( void ) : DrawableObjectImp()
{
  m_oDrawableClass  = NULL ;
  m_oDrawableObject = NULL ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObjectJoGL::~DrawableObjectJoGL( void )
{
  destroy() ;
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
void DrawableObjectJoGL::initializeDrawing( int figureIndex )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "initializeDrawing", "I", figureIndex ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endDrawing( void )
{   
  jniCallVoidFunctionSafe( m_oDrawableObject, "endDrawing", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::initializeShowing( int figureIndex )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "initializeShowing", "I", figureIndex ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObjectJoGL::endShowing( void )
{   
  jniCallVoidFunctionSafe( m_oDrawableObject, "endShowing", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
}
