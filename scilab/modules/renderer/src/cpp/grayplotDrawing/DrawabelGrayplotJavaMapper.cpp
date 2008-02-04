/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotJavaMapper.cpp                                   */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableGrayplot        */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplotJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableGrayplotJavaMapper::DrawableGrayplotJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_grayplotDrawing::DrawableGrayplotGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotJavaMapper::~DrawableGrayplotJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*----------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::drawGrayplot(const double xGrid[], int nbCol,
                                              const double yGrid[], int nbRow,
                                              double zCoord, const int colors[], int nbColors)
{
  long * javaColors = new long[nbColors];
  for (int i = 0; i < nbColors; i++)
  {
    javaColors[i] = colors[i];
  }

  m_pJavaObject->drawGrayplot((double *)xGrid, nbCol, (double *)yGrid, nbRow, zCoord, javaColors, nbColors);

  delete[] javaColors;
}
/*---------------------------------------------------------------------------------*/
}
