/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "BackTrihedronDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJavaMapper::BackTrihedronDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::BackTrihedronDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJavaMapper::~BackTrihedronDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::setBoxParameters(int hiddenAxisColor, float thickness)
{
  m_pJavaObject->setBoxParameters(hiddenAxisColor, thickness);
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJavaMapper::drawBox(double xMin, double xMax, double yMin,
                                            double yMax, double zMin, double zMax,
                                            int concealedCornerIndex)
{
  m_pJavaObject->drawBox(xMin, xMax, yMin, yMax, zMin, zMax, concealedCornerIndex);
}
/*--------------------------------------------------------------------------*/
}
