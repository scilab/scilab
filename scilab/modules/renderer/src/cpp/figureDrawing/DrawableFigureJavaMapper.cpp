/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Contains mapping of java method used by DrawableFigure
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableFigureJavaMapper.hxx"
#include "RenderingChecker.hxx"
#include <string.h>

extern "C"
{
#include "getScilabJavaVM.h"
}

using namespace std;

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableFigureJavaMapper::DrawableFigureJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_figureDrawing::DrawableFigureGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
DrawableFigureJavaMapper::~DrawableFigureJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::drawCanvas(void)
{
  m_pJavaObject->drawCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::drawBackground(void)
{
  m_pJavaObject->drawBackground(); 
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setLogicalOp(int logicOpIndex)
{
  m_pJavaObject->setLogicalOp(logicOpIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setColorMapData(const double rgbmat[], int nbColor)
{
  m_pJavaObject->setColorMapData((double *)rgbmat, nbColor);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::getColorMapData(double rgbmat[])
{
  int cmapSize = getColorMapSize();
  double * javaCmap = m_pJavaObject->getColorMapData();

  for ( int i = 0 ; i < 3 * cmapSize ; i++ ) {
    rgbmat[i] = javaCmap[i];
  }

  delete[] javaCmap;
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getColorMapSize(void)
{
  return m_pJavaObject->getColorMapSize();
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getCanvasWidth(void)
{
  return m_pJavaObject->getCanvasWidth();
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getCanvasHeight(void)
{
  return m_pJavaObject->getCanvasHeight();
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::setCanvasSize(int width, int height)
{
  return m_pJavaObject->setCanvasSize(width, height);
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowPosX(void)
{
  return m_pJavaObject->getWindowPosX();
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowPosY(void)
{
  return m_pJavaObject->getWindowPosY();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setWindowPosition(int posX, int posY)
{
  m_pJavaObject->setWindowPosition(posX, posY);
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowWidth(void)
{
  return m_pJavaObject->getWindowWidth();
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowHeight(void)
{
  return m_pJavaObject->getWindowHeight();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setWindowSize(int width, int height)
{
  m_pJavaObject->setWindowSize(width, height);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setInfoMessage(const char * infoMessage)
{
  m_pJavaObject->setInfoMessage((char *) infoMessage);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::getInfoMessage(char * infoMessage)
{
	char * javaMessage = m_pJavaObject->getInfoMessage();
	strcpy(infoMessage, javaMessage);
	delete[] javaMessage;
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getInfoMessageLength(void)
{
	char * javaMessage = m_pJavaObject->getInfoMessage();
	int res = (int) strlen(javaMessage);
	delete[] javaMessage;
	return res;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setAutoResizeMode(bool onOrOff)
{
  m_pJavaObject->setAutoResizeMode(onOrOff);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigureJavaMapper::getAutoResizeMode(void)
{
  return m_pJavaObject->getAutoResizeMode();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::getViewport(int viewport[4])
{
  int * javaViewport = m_pJavaObject->getViewport();
  viewport[0] = javaViewport[0];
  viewport[1] = javaViewport[1];
  viewport[2] = javaViewport[2];
  viewport[3] = javaViewport[3];

  delete[] javaViewport;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setViewport(const int viewport[4])
{
  m_pJavaObject->setViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setBackgroundColor(int colorIndex)
{
  m_pJavaObject->setBackgroundColor(colorIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::rubberBox(bool isClick, const int initialRect[4], int endRect[4], int * usedButton)
{
  int sizeJavaInitialRect = 0;
  if (initialRect != NULL)
  {
    sizeJavaInitialRect = 4;
  }
  int * javaRes = m_pJavaObject->rubberBox(isClick, (int *)initialRect, sizeJavaInitialRect);

  // javaRes = [x1,y1,x2,y2,button];
  for (int i = 0; i < 4; i++)
  {
    endRect[i] = javaRes[i];
  }

  *usedButton = javaRes[4];

  delete[] javaRes;

}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::interactiveZoom(long objectHandle)
{
	m_pJavaObject->interactiveZoom((long long) objectHandle);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setTitle(const char * title)
{
  m_pJavaObject->setTitle((char *) title);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::showWindow(void)
{
  m_pJavaObject->showWindow();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setNbSubwins(int nbSubwins)
{
  m_pJavaObject->setNbSubwins(nbSubwins);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::openGraphicCanvas(void)
{
	m_pJavaObject->openGraphicCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::closeGraphicCanvas(void)
{
	m_pJavaObject->closeGraphicCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::interactiveRotation(void)
{
	m_pJavaObject->interactiveRotation();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setUseSingleBuffer(bool useSingleBuffer)
{
	m_pJavaObject->setUseSingleBuffer(useSingleBuffer);
}
/*---------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getAntialiasingQuality(void)
{
	return m_pJavaObject->getAntialiasingQuality();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setAntialiasingQuality(int quality)
{
	m_pJavaObject->setAntialiasingQuality(quality);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigureJavaMapper::isAbleToCreateFigure(void)
{
  return org_scilab_modules_renderer_utils::RenderingChecker::isAbleToCreateWindow(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
}
