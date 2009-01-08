/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by DrawablePolyline
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
void DrawableArcJavaMapper::translate(const double translation[3])
{
  m_pJavaObject->translate(translation[0], translation[1], translation[2]);
}
/*---------------------------------------------------------------------------------*/
void DrawableArcJavaMapper::endTranslate(void)
{
  m_pJavaObject->endTranslate();
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
