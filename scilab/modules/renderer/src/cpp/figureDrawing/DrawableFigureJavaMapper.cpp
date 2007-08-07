/*------------------------------------------------------------------------*/
/* file: DrawableFigureJavaMapper.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableFigure          */
/*------------------------------------------------------------------------*/

#include "DrawableFigureJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

using namespace std;

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableFigureJavaMapper::DrawableFigureJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_figureDrawing::DrawableFigureGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
DrawableFigureJavaMapper::~DrawableFigureJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::drawCanvas(void)
{
  m_pJavaObject->drawCanvas();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::closeRenderingCanvas(void)
{
  m_pJavaObject->closeRenderingCanvas();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setBackgroundColor(int backgroundColor)
{
  m_pJavaObject->setBackgroundColor(backgroundColor);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setColorMapData(const double rgbmat[], int nbColor)
{
  // TODO
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::getColorMapData(double rgbmat[])
{
  // TODO
}
/*------------------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getCanvasWidth(void)
{
  return m_pJavaObject->getCanvasWidth();
}
/*------------------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getCanvasHeight(void)
{
  return m_pJavaObject->getCanvasHeight();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setCanvasSize(int width, int height)
{
  m_pJavaObject->setCanvasSize(width, height);
}
/*------------------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowPosX(void)
{
  return m_pJavaObject->getWindowPosX();
}
/*------------------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowPosY(void)
{
  return m_pJavaObject->getWindowPosY();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setWindowPosition(int posX, int posY)
{
  m_pJavaObject->setWindowPosition(posX, posY);
}
/*------------------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowWidth(void)
{
  return m_pJavaObject->getWindowWidth();
}
/*------------------------------------------------------------------------------------------*/
int DrawableFigureJavaMapper::getWindowHeight(void)
{
  return m_pJavaObject->getWindowHeight();
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setWindowSize(int width, int height)
{
  m_pJavaObject->setWindowSize(width, height);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigureJavaMapper::setInfoMessage(char * infoMessage)
{
  m_pJavaObject->setInfoMessage(infoMessage);
}
/*------------------------------------------------------------------------------------------*/

}
