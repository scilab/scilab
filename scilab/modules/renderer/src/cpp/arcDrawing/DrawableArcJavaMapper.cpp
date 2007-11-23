/*------------------------------------------------------------------------*/
/* file: DrawableArcJavaMapper.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawablePolyline        */
/*------------------------------------------------------------------------*/

#include "DrawableArcJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableArcJavaMapper::DrawableArcJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_arcDrawing::DrawableArcGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
DrawableArcJavaMapper::~DrawableArcJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*---------------------------------------------------------------------------------*/

}
