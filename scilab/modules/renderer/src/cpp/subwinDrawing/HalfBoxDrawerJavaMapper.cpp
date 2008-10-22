/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing java methods needed by HalfBoxDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "HalfBoxDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
HalfBoxDrawerJavaMapper::HalfBoxDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::HalfBoxDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
HalfBoxDrawerJavaMapper::~HalfBoxDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::setBoxParameters(int hiddenAxisColor,
                                               int lineColor, int lineStyle,
                                               float thickness)
{
  m_pJavaObject->setBoxParameters(hiddenAxisColor, lineColor,
                                  lineStyle, thickness);
}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJavaMapper::drawBox(double xMin, double xMax, double yMin,
                                      double yMax, double zMin, double zMax,
                                      int concealedCornerIndex)
{
  m_pJavaObject->drawBox(xMin, xMax, yMin, yMax, zMin, zMax, concealedCornerIndex);
}
/*--------------------------------------------------------------------------*/
}
