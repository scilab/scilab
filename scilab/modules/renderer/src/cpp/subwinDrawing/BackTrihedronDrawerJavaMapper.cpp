/*------------------------------------------------------------------------*/
/* file: BackTrihedronDrawerJavaMapper.cpp                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by BackTrihedronJoGL       */
/*------------------------------------------------------------------------*/


#include "BackTrihedronDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJavaMapper::BackTrihedronDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::BackTrihedronDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJavaMapper::~BackTrihedronDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::setBoxParameters(int hiddenAxisColor, int backgroundColor,
                                                     int lineStyle, float thickness)
{
  m_pJavaObject->setBoxParameters(hiddenAxisColor, backgroundColor, lineStyle, thickness);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::drawBox(double xMin, double xMax, double yMin,
                                            double yMax, double zMin, double zMax)
{
  m_pJavaObject->drawBox(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
}
