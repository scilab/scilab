/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by DrawableText
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableTextJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}


namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableTextJavaMapper::DrawableTextJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_textDrawing::DrawableTextGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
DrawableTextJavaMapper::~DrawableTextJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::translate(const double translation[3])
{
  m_pJavaObject->translate(translation[0], translation[1], translation[2]);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::endTranslate(void)
{
  m_pJavaObject->endTranslate();
}
/*----------------------------------------------------------------------*/
void DrawableTextJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void DrawableTextJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*---------------------------------------------------------------------------------*/
}
