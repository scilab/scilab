/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by DrawableSegs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSegsJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableSegsJavaMapper::DrawableSegsJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_segsDrawing::DrawableSegsGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
DrawableSegsJavaMapper::~DrawableSegsJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::translate(const double translation[3])
{
  m_pJavaObject->translate(translation[0], translation[1], translation[2]);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::endTranslate(void)
{
  m_pJavaObject->endTranslate();
}
/*----------------------------------------------------------------------*/
void DrawableSegsJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*---------------------------------------------------------------------------------*/
}
