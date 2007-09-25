/*------------------------------------------------------------------------*/
/* file: DrawableTextJavaMapper.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableText            */
/*------------------------------------------------------------------------*/

#include "DrawableTextJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableTextJavaMapper::DrawableTextJavaMapper(void)
{
  //m_pJavaObject = new org_scilab_modules_renderer_textDrawing::DrawableTextGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
DrawableTextJavaMapper::~DrawableTextJavaMapper(void)
{
  //delete m_pJavaObject;
  //m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::display(void)
{
  //m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::initializeDrawing(int figureIndex)
{
  //m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::endDrawing(void)
{
  //m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::show(int figureIndex)
{
  //m_pJavaObject->show(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::destroy(int parentFigureIndex)
{
  //m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::setFigureIndex(int figureIndex)
{
  //m_pJavaObject->setFigureIndex(figureIndex);
}
/*----------------------------------------------------------------------*/
void DrawableTextJavaMapper::clipX(double xMin, double xMax)
{
  //m_pJavaObject->clipX(xMin, xMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::clipY(double yMin, double yMax)
{
  //m_pJavaObject->clipY(yMin, yMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::clipZ(double zMin, double zMax)
{
  //m_pJavaObject->clipZ(zMin, zMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::unClip(void)
{
  //m_pJavaObject->unClip();
}
/*------------------------------------------------------------------------------------------*/
}
