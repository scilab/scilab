/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by DrawableSurface
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSurfaceJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}


namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableSurfaceJavaMapper::DrawableSurfaceJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_surfaceDrawing::DrawableSurfaceGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
DrawableSurfaceJavaMapper::~DrawableSurfaceJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::translate(const double translation[3])
{
  m_pJavaObject->translate(translation[0], translation[1], translation[2]);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::endTranslate(void)
{
  m_pJavaObject->endTranslate();
}
/*----------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*---------------------------------------------------------------------------------*/
}
