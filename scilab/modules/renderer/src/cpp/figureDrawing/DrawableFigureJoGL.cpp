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
  jniCallVoidFunctionSafe( m_oDrawableObject, "display", "" ) ;
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
void DrawableFigureJoGL::setBackgroundColor( int backgroundColor )
{
  jniCallVoidFunctionSafe( m_oDrawableObject, "setBackgroundColor", "I", backgroundColor ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setColorMap( const double rgbMat[], int nbColor )
{
  jdoubleArray javaCmap = NULL;
  jniUpdateCurrentEnv();
  javaCmap = jniCreateDoubleArrayCopy(rgbMat, nbColor) ;
  jniCallVoidFunction( m_oDrawableObject, "setColorMapData", "[D", javaCmap ) ;
  jniDeleteLocalEntity(javaCmap) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getColorMap( double rgbMat[] )
{
  jmethodID colorMapMethod = NULL ;
  jdoubleArray javaCMap = NULL ;
  jniUpdateCurrentEnv();
  JNIEnv * curEnv = jniGetCurrentJavaEnv() ;

  colorMapMethod = curEnv->GetMethodID( m_oDrawableClass, "getColorMapData", "([D)V" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getColorMapData" ) ;
    return ;
  }

  javaCMap = curEnv->NewDoubleArray( sciGetNumColors(getDrawer()->getDrawedObject()) * 3 ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Unable to allocate colormap, memory full.\n" ) ;
    return ;
  }

  curEnv->CallObjectMethod( m_oDrawableObject, colorMapMethod, javaCMap ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getColorMapData" ) ;
    return ;
  }

  jniCopyJavaDoubleArray( javaCMap, rgbMat ) ;
  
  jniDeleteLocalEntity(javaCMap) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getSize( int size[2] )
{
  jmethodID getSizeMethod = NULL ;
  jniUpdateCurrentEnv();
  JNIEnv * curEnv = jniGetCurrentJavaEnv() ;

  // width
  getSizeMethod = curEnv->GetMethodID( m_oDrawableClass, "getCanvasWidth", "()I" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getCanvasWidth" ) ;
    return ;
  }

  size[0] = curEnv->CallIntMethod( m_oDrawableObject, getSizeMethod ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getCanvasWidth" ) ;
    return ;
  }

  // height
  getSizeMethod = curEnv->GetMethodID( m_oDrawableClass, "getCanvasHeight", "()I" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getCanvasHeight" ) ;
    return ;
  }

  size[1] = curEnv->CallIntMethod( m_oDrawableObject, getSizeMethod ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getCanvasHeight" ) ;
    return ;
  }


}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setSize( const int size[2] )
{
  jniCallVoidFunctionSafe(m_oDrawableObject, "setCanvasSize", "II", size[0], size[1] ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowPosition( int pos[2] )
{
  jmethodID getPosMethod = NULL ;
  jniUpdateCurrentEnv();
  JNIEnv * curEnv = jniGetCurrentJavaEnv() ;

  // X coord
  getPosMethod = curEnv->GetMethodID( m_oDrawableClass, "getWindowPosX", "()I" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowPosX" ) ;
    return ;
  }

  pos[0] = curEnv->CallIntMethod( m_oDrawableObject, getPosMethod ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowPosX" ) ;
    return ;
  }

  // Y coord
  getPosMethod = curEnv->GetMethodID( m_oDrawableClass, "getWindowPosY", "()I" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowPosY" ) ;
    return ;
  }

  pos[1] = curEnv->CallIntMethod( m_oDrawableObject, getPosMethod ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowPosY" ) ;
    return ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowPosition( const int pos[2] )
{
  jniCallVoidFunctionSafe(m_oDrawableObject, "setWindowPosition", "II", pos[0], pos[1] ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowSize( int size[2] )
{
  jmethodID getSizeMethod = NULL ;
  jniUpdateCurrentEnv();
  JNIEnv * curEnv = jniGetCurrentJavaEnv() ;

  // width
  getSizeMethod = curEnv->GetMethodID( m_oDrawableClass, "getWindowWidth", "()I" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowWidth" ) ;
    return ;
  }

  size[0] = curEnv->CallIntMethod( m_oDrawableObject, getSizeMethod ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowWidth" ) ;
    return ;
  }

  // height
  getSizeMethod = curEnv->GetMethodID( m_oDrawableClass, "getWindowHeight", "()I" ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowHeight" ) ;
    return ;
  }

  size[1] = curEnv->CallIntMethod( m_oDrawableObject, getSizeMethod ) ;
  if ( !jniCheckLastCall(TRUE) )
  {
    Scierror( 999, "Error when calling function %s.\r\n", "getWindowHeight" ) ;
    return ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowSize( const int size[2] )
{
  jniCallVoidFunctionSafe(m_oDrawableObject, "setWindowSize", "II", size[0], size[1] ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setInfoMessage( const char * message )
{
  jstring infoMessage = jniCreateStringCopy( message ) ;
  jniCallVoidFunctionSafe( m_oDrawableObject, "setInfoMessage", "Ljava/lang/String;", infoMessage ) ;
  jniDeleteLocalEntity( infoMessage ) ;
}
/*------------------------------------------------------------------------------------------*/
}
