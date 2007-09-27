/*------------------------------------------------------------------------*/
/* file: StandardTextDrawerJavaMapper.cpp                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by StandardTextDrawerJoGL  */
/*------------------------------------------------------------------------*/


#include "StandardTextDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper::StandardTextDrawerJavaMapper(void)
{
  //m_pJavaObject = new org_scilab_modules_renderer_textDrawing::StandardTextDrawerGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper::~StandardTextDrawerJavaMapper(void)
{
  //delete m_pJavaObject;
  //m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::display(void)
{
  //m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  //m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::endDrawing(void)
{
  //m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::show(int figureIndex)
{
  //m_pJavaObject->show(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::destroy(int figureIndex)
{
  //m_pJavaObject->destroy(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  //m_pJavaObject->setFigureIndex(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setTextParameters(int textAlignement, int color, int fontStyle, double fontSize)
{
  
}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setTextContent(const char text[], int nbRow, int nbCol)
{

}
/*------------------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::drawTextContent(double centerX, double centerY, double centerZ)
{

}
/*------------------------------------------------------------------------------------------*/
double * StandardTextDrawerJavaMapper::getBoundingRectangle(void)
{
  return NULL;
}
/*------------------------------------------------------------------------------------------*/
long * StandardTextDrawerJavaMapper::getScreenBoundingBox(void)
{
  return NULL;
}
/*------------------------------------------------------------------------------------------*/
}
