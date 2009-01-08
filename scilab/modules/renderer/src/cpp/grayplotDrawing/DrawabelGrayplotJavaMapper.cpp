/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by DrawableGrayplot 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::translate(const double translation[3])
{
  m_pJavaObject->translate(translation[0], translation[1], translation[2]);
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJavaMapper::endTranslate(void)
{
  m_pJavaObject->endTranslate();
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
  m_pJavaObject->drawGrayplot((double *)xGrid, nbCol, (double *)yGrid, nbRow, zCoord, (int *)colors, nbColors);
}
/*---------------------------------------------------------------------------------*/
}
