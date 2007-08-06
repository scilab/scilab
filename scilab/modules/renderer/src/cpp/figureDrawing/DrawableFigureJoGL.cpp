/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

#include "DrawableFigure.h"
#include "DrawableFigureBridge.h"
#include "DrawableFigureJoGL.h"
extern "C"
{
#include "GetProperty.h"
#include "DrawObjects.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
}

#include <string.h>

using namespace std;

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::DrawableFigureJoGL( DrawableFigure * drawer )
  : DrawableObjectJoGL(drawer)
{
  
}
/*------------------------------------------------------------------------------------------*/
DrawableFigureJoGL::~DrawableFigureJoGL( void )
{
  closeRenderingCanvas() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::drawCanvas( void )
{
  getFigureJavaMapper()->drawCanvas();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::openRenderingCanvas( int figureIndex )
{
  m_pJavaWindow = new org_scilab_modules_gui_graphicWindow::ScilabGraphicWindow(getScilabJavaVM());
  getFigureJavaMapper()->setFigureIndex(sciGetNum(getDrawer()->getDrawedObject()));
  m_pJavaWindow->setFigureIndex(sciGetNum(getDrawer()->getDrawedObject()));
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeRenderingCanvas( void )
{
  if ( getFigureJavaMapper() != NULL )
  {
    getFigureJavaMapper()->closeRenderingCanvas();
  }
  if ( m_pJavaWindow != NULL )
  {
    delete m_pJavaWindow;
    m_pJavaWindow = NULL;
  }
  destroy() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setBackgroundColor( int backgroundColor )
{
  getFigureJavaMapper()->setBackgroundColor(backgroundColor);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setColorMap( const double rgbMat[], int nbColor )
{
  getFigureJavaMapper()->setColorMapData(rgbMat, nbColor);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getColorMap( double rgbMat[] )
{
  getFigureJavaMapper()->getColorMapData(rgbMat);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getSize( int size[2] )
{

  size[0] = getFigureJavaMapper()->getCanvasWidth();
  size[1] = getFigureJavaMapper()->getCanvasHeight();

}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setSize( const int size[2] )
{
  getFigureJavaMapper()->setCanvasSize(size[0], size[1]);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowPosition( int pos[2] )
{
  pos[0] = getFigureJavaMapper()->getWindowPosX();
  pos[1] = getFigureJavaMapper()->getWindowPosY();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowPosition( const int pos[2] )
{
  getFigureJavaMapper()->setWindowPosition(pos[0], pos[1]);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowSize( int size[2] )
{
  size[0] = getFigureJavaMapper()->getWindowWidth();
  size[1] = getFigureJavaMapper()->getWindowHeight();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowSize( const int size[2] )
{
  getFigureJavaMapper()->setWindowSize(size[0], size[1]);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setInfoMessage( const char * message )
{
  // we need to create a not const char
  char * infoMessage = new char[strlen(message)+1];
  strcpy(infoMessage, message);
  //getFigureJavaMapper()->setInfoMessage(infoMessage);
  delete[] infoMessage;
}
/*------------------------------------------------------------------------------------------*/
DrawableFigure * DrawableFigureJoGL::getFigureDrawer( void )
{
  return dynamic_cast<DrawableFigure *>(getDrawer()) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableFigureJavaMapper * DrawableFigureJoGL::getFigureJavaMapper(void)
{
  return dynamic_cast<DrawableFigureJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
