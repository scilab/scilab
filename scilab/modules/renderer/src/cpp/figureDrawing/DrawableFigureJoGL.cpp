/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

#include "DrawableFigure.h"
#include "DrawableFigureImp.h"
#include "DrawableFigureJoGL.h"
extern "C"
{
#include "Xcall1.h"
#include "GetProperty.h"
#include "periScreen.h" /* should be removed */
#include "DrawObjects.h"
#include "getScilabJavaVM.h"
#include "JniUtils.h"
#include "Scierror.h"
}

#include <stdio.h>

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::DrawableFigureJoGL( DrawableFigure * drawer )
  : DrawableObjectImp(drawer), DrawableFigureImp(drawer), DrawableObjectJoGL(drawer, "org/scilab/modules/renderer/figureDrawing/DrawableFigureJoGL") {}
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
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "display", "()V" ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::openRenderingCanvas( int figureIndex )
{
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "openRenderingCanvas", "(I)V", figureIndex ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeRenderingCanvas( void )
{
  if ( m_oDrawableObject != NULL )
  {
    jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "closeRenderingCanvas", "()V" ) ;
  }
  DrawableObjectJoGL::destroy() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setBackgroundColor( int backgroundColor )
{
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "setBackgroundColor", "(I)V", backgroundColor ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setColorMap( const double rgbMat[], int nbColor )
{
  jdoubleArray javaCmap = NULL;
  jniUpdateCurrentEnv();
  javaCmap = jniCreateDoubleArrayCopy(rgbMat, nbColor) ;
  
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "setColorMapData", "([D)V", javaCmap ) ;
  jniDeleteLocalEntity(javaCmap) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getColorMap( double rgbMat[] )
{
  jdoubleArray javaCMap = NULL ;
  jniUpdateCurrentEnv();
  JNIEnv * curEnv = jniGetCurrentJavaEnv() ;

  javaCMap = curEnv->NewDoubleArray( sciGetNumColors(getDrawer()->getDrawedObject()) * 3 ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to allocate colormap, memory full.\n" ) ;
    return ;
  }

  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getColorMapData", "([D)V", javaCMap) ;


  jniCopyJavaDoubleArray( javaCMap, rgbMat ) ;
  
  jniDeleteLocalEntity(javaCMap) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getSize( int size[2] )
{

  size[0] = jniGetIntValue(jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getCanvasWidth", "()I")) ;
  size[1] = jniGetIntValue(jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getCanvasWidth", "()I")) ;

}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setSize( const int size[2] )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "setCanvasSize", "(II)V", size[0], size[1] ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowPosition( int pos[2] )
{
  pos[0] = jniGetIntValue(jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getWindowPosX", "()I")) ;
  pos[1] = jniGetIntValue(jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getWindowPosY", "()I")) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowPosition( const int pos[2] )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "setWindowPosition", "(II)V", pos[0], pos[1] ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowSize( int size[2] )
{
  size[0] = jniGetIntValue(jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getWindowWidth", "()I")) ;
  size[1] = jniGetIntValue(jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "getWindowHeight", "()I")) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowSize( const int size[2] )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "setWindowSize", "(II)V", size[0], size[1] ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setInfoMessage( const char * message )
{
  jstring infoMessage = jniCreateStringCopy( message ) ;
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "setInfoMessage", "(Ljava/lang/String;)V", infoMessage ) ;
  jniDeleteLocalEntity( infoMessage ) ;
}
/*------------------------------------------------------------------------------------------*/
}
