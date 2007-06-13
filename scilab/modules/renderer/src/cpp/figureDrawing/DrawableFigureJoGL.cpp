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
  : DrawableObjectImp(drawer), DrawableFigureImp( drawer ), DrawableObjectJoGL(drawer, "org/scilab/modules/renderer/figureDrawing/DrawableFigureJoGL") {}
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
}
