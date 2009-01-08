/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Class containing the driver dependant routines to draw a figure object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFigure.h"
#include "DrawableFigureBridge.h"
#include "DrawableFigureJoGL.h"
#include "GraphicSynchronizerInterface.h"

extern "C"
{
#include "GetProperty.h"
#include "DrawObjects.h"
#include "getScilabJavaVM.h"
#include "CallFigure.h"
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
  // do nothing here, figure is alredy closed
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::drawCanvas( void )
{
  getFigureJavaMapper()->drawCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::createVisualFigure( int figureIndex )
{
	// add the figure in the hashmap
	getFigureJavaMapper()->setFigureIndex(figureIndex);

	// create the window
	newFigure(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeVisualFigure( void )
{
  // disable synchrnonization here to avoid deadlocks
	disableFigureSynchronization(m_pDrawer->getDrawedObject());
  DrawableObjectJoGL::destroy() ;
	enableFigureSynchronization(m_pDrawer->getDrawedObject());
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setFigureParameters(void)
{
  sciPointObj * pFigure = getFigureDrawer()->getDrawedObject();
  getFigureJavaMapper()->setLogicalOp(sciGetXorMode(pFigure));
  getFigureJavaMapper()->setNbSubwins(sciGetNbTypedObjects(pFigure, SCI_SUBWIN));
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::drawBackground(void)
{
  getFigureJavaMapper()->drawBackground();
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
int DrawableFigureJoGL::setSize( const int size[2] )
{
  return getFigureJavaMapper()->setCanvasSize(size[0], size[1]);
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
  getFigureJavaMapper()->setInfoMessage(message);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getInfoMessage(char * message)
{
	getFigureJavaMapper()->getInfoMessage(message);
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJoGL::getInfoMessageLength(void)
{
	return getFigureJavaMapper()->getInfoMessageLength();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setAutoResizeMode(bool onOrOff)
{
  getFigureJavaMapper()->setAutoResizeMode(onOrOff);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigureJoGL::getAutoResizeMode(void)
{
  return getFigureJavaMapper()->getAutoResizeMode();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::getViewport(int viewport[4])
{
  getFigureJavaMapper()->getViewport(viewport);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setViewport(const int viewport[4])
{
  getFigureJavaMapper()->setViewport(viewport);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setBackgroundColor(int backColor)
{
  getFigureJavaMapper()->setBackgroundColor(backColor);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::rubberBox(bool isClick, const int initialRect[4], int endRect[4], int * usedButton)
{
  getFigureJavaMapper()->rubberBox(isClick, initialRect, endRect, usedButton);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::interactiveZoom(void)
{
	getFigureJavaMapper()->interactiveZoom(sciGetHandle(getFigureDrawer()->getDrawedObject()));
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
void DrawableFigureJoGL::destroy( void )
{
  // do nothing otherwise destroy will be called after window closing
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setTitle( const char * title )
{
  getFigureJavaMapper()->setTitle(title);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::interactiveRotation(void)
{
	getFigureJavaMapper()->interactiveRotation();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::showWindow(void)
{
  getFigureJavaMapper()->showWindow();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::openGraphicCanvas(void)
{
	getFigureJavaMapper()->openGraphicCanvas();

}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::closeGraphicCanvas(void)
{
	getFigureJavaMapper()->closeGraphicCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJoGL::setUseSingleBuffer(bool useSingleBuffer)
{
	getFigureJavaMapper()->setUseSingleBuffer(useSingleBuffer);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigureJoGL::isAbleToCreateFigure(void)
{
  return DrawableFigureJavaMapper::isAbleToCreateFigure();
}
/*---------------------------------------------------------------------------------*/
}
