/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by PolylineFillDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "PolylineFillDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineFillDrawerJavaMapper::PolylineFillDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineFillDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
PolylineFillDrawerJavaMapper::~PolylineFillDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::setBackColor(int color)
{
  m_pJavaObject->setBackColor(color);
}
/*---------------------------------------------------------------------------------*/
void PolylineFillDrawerJavaMapper::drawPolyline(const double xCoords[], const double yCoords[], const double zCoords[], int nbCoords)
{
  m_pJavaObject->drawPolyline((double *)xCoords, nbCoords,
                              (double *)yCoords, nbCoords,
                              (double *)zCoords, nbCoords);
}
/*---------------------------------------------------------------------------------*/
}
