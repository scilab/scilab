/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
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
void DrawableFigureJavaMapper::closeRenderingCanvas(void)
{
  m_pJavaObject->closeRenderingCanvas();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::drawBackground(int colorIndex)
{
  m_pJavaObject->drawBackground(colorIndex); 
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
void DrawableFigureJavaMapper::setCanvasSize(int width, int height)
{
  m_pJavaObject->setCanvasSize(width, height);
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
void DrawableFigureJavaMapper::setInfoMessage(char * infoMessage)
{
  m_pJavaObject->setInfoMessage(infoMessage);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setPixmapMode(bool onOrOff)
{
  m_pJavaObject->setPixmapMode(onOrOff);  
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigureJavaMapper::getPixmapMode(void)
{
 return m_pJavaObject->getPixmapMode();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setRenderingEnable(bool isEnable)
{
  m_pJavaObject->setIsRenderingEnable(isEnable);
}
/*---------------------------------------------------------------------------------*/

}
