/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

#include "DrawableFigureJoGL.h"
extern "C"
{
#include "Xcall1.h"
#include "GetProperty.h"
#include "periScreen.h" /* should be removed */
#include "DrawObjects.h"
#include "getScilabJavaVM.h"
#include "JniUtils.h"
}

#include <stdio.h>

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::DrawableFigureJoGL( DrawableFigure * drawer )
  : DrawableFigureImp( drawer ), DrawableObjectJoGL("org/scilab/modules/graphics/figureDrawing/DrawableFigureJoGL") {}
/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::~DrawableFigureJoGL( void )
{
  closeRenderingCanvas() ;
  destroy() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::drawCanvas( void )
{
  // We call the display function to be sure to be in the right context
  jniCallVoidFunctionSafe( m_oDrawableObject, "display", "" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::updateInfoMessage( void )
{
  jstring infoMessage = jniCreateStringCopy( sciGetInfoMessage( m_pDrawer->getDrawedObject() ) ) ;
  jniCallVoidFunctionSafe( m_oDrawableObject, "updateInfoMessage", "Ljava/lang/String;", infoMessage ) ;
  jniDeleteLocalEntity( infoMessage ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::openRenderingCanvas( int figureIndex )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "openRenderingCanvas", "I", figureIndex ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeRenderingCanvas( void )
{
  if ( m_oDrawableObject != NULL )
  {
    jniCallVoidFunctionSafe( m_oDrawableObject, "closeRenderingCanvas", "" ) ;
  }
  DrawableObjectJoGL::destroy() ;
}
/*------------------------------------------------------------------------------------------*/
}
