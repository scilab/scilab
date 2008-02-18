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
}

#include <string.h>

using namespace std;

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFigureJoGL::DrawableFigureJoGL( DrawableFigure * drawer )
  : DrawableObjectJoGL(drawer)
{
  
}
/*---------------------------------------------------------------------------------*/
DrawableFigureJoGL::~DrawableFigureJoGL( void )
{
  closeRenderingCanvas() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::drawCanvas( void )
{
  getFigureJavaMapper()->drawCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::openRenderingCanvas( int figureIndex )
{
  m_pJavaWindow = new org_scilab_modules_gui_graphicWindow::ScilabGraphicWindow(getScilabJavaVM());
  getFigureJavaMapper()->setFigureIndex(figureIndex);
  m_pJavaWindow->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeRenderingCanvas( void )
{
  destroy() ;
  if ( m_pJavaWindow != NULL )
  {
    delete m_pJavaWindow;
    m_pJavaWindow = NULL;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setFigureParameters(void)
{
  sciPointObj * pFigure = getFigureDrawer()->getDrawedObject();
  getFigureJavaMapper()->setLogicalOp(sciGetXorMode(pFigure));
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::drawBackground(void)
{
  sciPointObj * pFigure = getFigureDrawer()->getDrawedObject();
  getFigureJavaMapper()->drawBackground(sciGetGraphicContext(pFigure)->backgroundcolor);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setColorMap( const double rgbMat[], int nbColor )
{
  getFigureJavaMapper()->setColorMapData(rgbMat, nbColor);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getColorMap( double rgbMat[] )
{
  getFigureJavaMapper()->getColorMapData(rgbMat);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getSize( int size[2] )
{

  size[0] = getFigureJavaMapper()->getCanvasWidth();
  size[1] = getFigureJavaMapper()->getCanvasHeight();

}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setSize( const int size[2] )
{
  getFigureJavaMapper()->setCanvasSize(size[0], size[1]);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowPosition( int pos[2] )
{
  pos[0] = getFigureJavaMapper()->getWindowPosX();
  pos[1] = getFigureJavaMapper()->getWindowPosY();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowPosition( const int pos[2] )
{
  getFigureJavaMapper()->setWindowPosition(pos[0], pos[1]);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getWindowSize( int size[2] )
{
  size[0] = getFigureJavaMapper()->getWindowWidth();
  size[1] = getFigureJavaMapper()->getWindowHeight();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setWindowSize( const int size[2] )
{
  getFigureJavaMapper()->setWindowSize(size[0], size[1]);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setInfoMessage( const char * message )
{
  // we need to create a not const char
  char * infoMessage = new char[strlen(message)+1];
  strcpy(infoMessage, message);
  getFigureJavaMapper()->setInfoMessage(infoMessage);
  delete[] infoMessage;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setPixmapMode(bool onOrOff)
{
  getFigureJavaMapper()->setPixmapMode(onOrOff);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigureJoGL::getPixmapMode(void)
{
  return getFigureJavaMapper()->getPixmapMode();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setRenderingEnable(bool isEnable)
{
  getFigureJavaMapper()->setRenderingEnable(isEnable);
}
/*---------------------------------------------------------------------------------*/
DrawableFigure * DrawableFigureJoGL::getFigureDrawer( void )
{
  return dynamic_cast<DrawableFigure *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableFigureJavaMapper * DrawableFigureJoGL::getFigureJavaMapper(void)
{
  return dynamic_cast<DrawableFigureJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
